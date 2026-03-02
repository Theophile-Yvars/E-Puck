#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "sensor_msgs/msg/range.hpp"

using namespace std::chrono_literals;

class EpuckController : public rclcpp::Node {
public:
    EpuckController() : Node("epuck_controller") {
        RCLCPP_INFO(this->get_logger(), "=== Contrôleur e-Puck (Capteurs Arrière Ignorés) ===");

        /*
        Publication sur le topic /cmd_vel pour envoyer les commandes de vitesse angulaire.
        */
        pub_vitesse = this->create_publisher<geometry_msgs::msg::TwistStamped>("/cmd_vel", 10);

        /*
        Abonnement aux capteurs de proximité (ps0 à ps7).
         - ps0, ps1 : avant/droite
         - ps7, ps6 : avant/gauche
        */
        std::string topic;

        topic = "/ps0";
        subs_distance[0] = this->create_subscription<sensor_msgs::msg::Range>(topic, 10, [this](const sensor_msgs::msg::Range::SharedPtr msg) {
            this->dist_values[0] = msg->range;
        });
        topic = "/ps1";
        subs_distance[1] = this->create_subscription<sensor_msgs::msg::Range>(topic, 10, [this](const sensor_msgs::msg::Range::SharedPtr msg) {
            this->dist_values[1] = msg->range;
        });
        topic = "/ps7";
        subs_distance[7] = this->create_subscription<sensor_msgs::msg::Range>(topic, 10, [this](const sensor_msgs::msg::Range::SharedPtr msg) {
            this->dist_values[7] = msg->range;
        });
        topic = "/ps6";
        subs_distance[6] = this->create_subscription<sensor_msgs::msg::Range>(topic, 10, [this](const sensor_msgs::msg::Range::SharedPtr msg) {
            this->dist_values[6] = msg->range;
        });

        /*
        Capteur situé à l'avant, il peut détecter des obstacles plus loin que les capteurs de proximité.
        */
        sub_tof = this->create_subscription<sensor_msgs::msg::Range>("/tof", 10, [this](const sensor_msgs::msg::Range::SharedPtr msg) {
            this->tof_val = msg->range;
        });

        /*
        Timer de contrôle à 20 Hz (50 ms) pour exécuter la boucle de contrôle.
        */
        timer = this->create_wall_timer(50ms, std::bind(&EpuckController::control_loop, this));
    }

private:
    void control_loop() {
        auto msg = geometry_msgs::msg::TwistStamped();
        msg.header.stamp = this->get_clock()->now();
        msg.header.frame_id = "base_link";

        /*
        On fait une moyenne des capteurs avant/droite et avant/gauche pour avoir une estimation plus stable de la proximité.
         - prox_droite : moyenne de ps0, ps1, ps2
         - prox_gauche : moyenne de ps7, ps6
         (ps4 et ps5 sont ignorés car à l'arrière)
        */
        float prox_droite = (dist_values[0] + dist_values[1]) / 2.0;
        float prox_gauche = (dist_values[6] + dist_values[7]) / 2.0;

        float v_lin = 0.5; // Vitesse linéaire de base
        float v_ang = 0.0; // Vitesse angulaire de base
        float seuil_urgence = 0.05; // Seuil de distance pour considérer un obstacle comme imminent (5 cm)

        /*
        On vérifie 0, 1 (avant/droite) et 6, 7 (avant/gauche). 

        */
       bool obstacle_imminent = false;
        for(int i : {0, 1, 6, 7}) {
            if (dist_values[i] < seuil_urgence && dist_values[i] > 0.001) {
                obstacle_imminent = true;
                break;
            }
        }

        if (obstacle_imminent) {
            v_lin = 0.0;
            v_ang = (prox_droite < prox_gauche) ? 1.5 : -1.5;
            RCLCPP_WARN(this->get_logger(), "OBSTACLE AVANT ! Pivotement");
        } 
        else if (tof_val < 0.20) {
            v_lin = 0.2; 
            v_ang = (prox_droite < prox_gauche) ? 0.7 : -0.7; // Ralentissement et pivotement plus doux que pour un obstacle imminent.
            RCLCPP_WARN(this->get_logger(), "Obstacle détecté par ToF à %.2f m, ralentissement et pivotement", tof_val);
        } 
        else {
            v_lin = 0.6; 
            v_ang = (prox_droite - prox_gauche) * 2.0; // Plus la différence est grande, plus le robot tourne pour s'éloigner de l'obstacle.
            //RCLCPP_INFO(this->get_logger(), "Pas d'obstacle imminent. Vitesse linéaire: %.2f, Vitesse angulaire: %.2f", v_lin, v_ang);
        }

        msg.twist.linear.x = v_lin;
        msg.twist.angular.z = v_ang;
        pub_vitesse->publish(msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr pub_vitesse;
    rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr subs_distance[8];
    rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr sub_tof;
    rclcpp::TimerBase::SharedPtr timer;

    /*
    Il y a 8 capteurs de proximité (ps0 à ps7), mais on n'utilise que les 4 avant (ps0, ps1, ps6, ps7).
     - ps0, ps1 : avant/droite
     - ps7, ps6 : avant/gauche
    */
    float dist_values[8] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    
    float tof_val = 2.0;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<EpuckController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
