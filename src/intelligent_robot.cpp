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
        Abandon des capteurs de proximité arrière (ps4 et ps5). 
         - ps0, ps1, ps2 : avant/droite
         - ps3 : avant/droite (mais plus proche du centre)
         - ps6, ps7 : avant/gauche
         - ps4, ps5 : arrière (ignorés)
        */
        for (int i = 0; i < 8; i++) {
            std::string topic = "/ps" + std::to_string(i);
            subs_distance[i] = this->create_subscription<sensor_msgs::msg::Range>(
                topic, 10, [this, i](const sensor_msgs::msg::Range::SharedPtr msg) {
                    this->dist_values[i] = msg->range;
                });
        }

        /*
        Abandon du capteur ToF. 
        Capteur situé à l'avant, il peut détecter des obstacles plus loin que les capteurs de proximité.
        */
        sub_tof = this->create_subscription<sensor_msgs::msg::Range>(
            "/tof", 10, [this](const sensor_msgs::msg::Range::SharedPtr msg) {
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

        // --- FILTRAGE DES CAPTEURS ---
        // On ignore ps4 et ps5 (arrière). 
        // Droite : ps0, ps1, ps2
        // Gauche : ps7, ps6 (ps5 est ignoré car arrière)
        float prox_droite = (dist_values[0] + dist_values[1] + dist_values[2]) / 3.0;
        float prox_gauche = (dist_values[6] + dist_values[7]) / 2.0;

        float v_lin = 0.5;
        float v_ang = 0.0;
        float seuil_urgence = 0.05; 

        // 1. Détection d'obstacle proche (uniquement sur les 6 capteurs avant/côté)
        bool obstacle_imminent = false;
        // On vérifie 0, 1, 2, 3 (avant/droite) et 6, 7 (avant/gauche). On saute 4 et 5.
        for(int i : {0, 1, 2, 3, 6, 7}) {
            if (dist_values[i] < seuil_urgence && dist_values[i] > 0.001) {
                obstacle_imminent = true;
                break;
            }
        }

        if (obstacle_imminent) {
            v_lin = 0.0;
            // Si c'est plus bouché à droite, on tourne à gauche
            v_ang = (prox_droite < prox_gauche) ? 1.5 : -1.5;
            RCLCPP_WARN(this->get_logger(), "OBSTACLE AVANT ! Pivotement");
        } 
        else if (tof_val < 0.20) {
            v_lin = 0.2; 
            v_ang = (prox_droite < prox_gauche) ? 0.7 : -0.7;
        } 
        else {
            v_lin = 0.6; 
            v_ang = (prox_droite - prox_gauche) * 2.0;
        }

        msg.twist.linear.x = v_lin;
        msg.twist.angular.z = v_ang;
        pub_vitesse->publish(msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr pub_vitesse;
    rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr subs_distance[8];
    rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr sub_tof;
    rclcpp::TimerBase::SharedPtr timer;

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
