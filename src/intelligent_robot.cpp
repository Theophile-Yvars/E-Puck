#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp" // Changé pour TwistStamped
#include "sensor_msgs/msg/range.hpp"

using namespace std::chrono_literals;

class EpuckController : public rclcpp::Node {
public:
    EpuckController() : Node("epuck_controller") {
        RCLCPP_INFO(this->get_logger(), "=== Lancement du Contrôleur TwistStamped ===");

        // Publisher : Type TwistStamped pour ROS 2 Jazzy
        pub_vitesse = this->create_publisher<geometry_msgs::msg::TwistStamped>("/cmd_vel", 10);

        // Subscribers : Capteurs de distance
        for (int i = 0; i < 8; i++) {
            std::string topic = "/ps" + std::to_string(i);
            subs_distance[i] = this->create_subscription<sensor_msgs::msg::Range>(
                topic, 10, [this, i](const sensor_msgs::msg::Range::SharedPtr msg) {
                    this->dist_values[i] = msg->range;
                });
        }

        // Boucle de contrôle à 20Hz (toutes les 50ms)
        timer = this->create_wall_timer(50ms, std::bind(&EpuckController::control_loop, this));
    }

private:
    void control_loop() {
        auto msg = geometry_msgs::msg::TwistStamped();

        // Remplissage obligatoire du Header pour TwistStamped
        msg.header.stamp = this->get_clock()->now();
        msg.header.frame_id = "base_link";

        // Lecture des capteurs avant
        float avant_droit = dist_values[0];
        float avant_gauche = dist_values[7];
        float seuil = 0.05; // 5 cm

        if (avant_droit < seuil || avant_gauche < seuil) {
            // Obstacle : On tourne
            RCLCPP_WARN(this->get_logger(), "Obstacle détecté !");
            msg.twist.linear.x = 0.0;
            msg.twist.angular.z = 0.5;
        } else {
            // Libre : On avance
            msg.twist.linear.x = 0.1;
            msg.twist.angular.z = 0.0;
        }

        pub_vitesse->publish(msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr pub_vitesse;
    rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr subs_distance[8];
    rclcpp::TimerBase::SharedPtr timer;
    float dist_values[8] = {0.0};
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<EpuckController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
