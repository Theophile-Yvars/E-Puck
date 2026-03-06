#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "sensor_msgs/msg/range.hpp"
#include "std_msgs/msg/int32.hpp"


class EPunkController : public rclcpp::Node {
public:
    EPunkController() : Node("epuck-v2"){
        pub_led0 = this->create_publisher<std_msgs::msg::Int32>("/led0", 1);
        timer = this->create_wall_timer(50ms, std::bind(&EpuckController::control_loop, this));
    }

private:
    void control_loop() {
        if(counter < 10) {
            message.data = 1; // ALLUMER
        } else {
            message.data = 0; // ÉTEINDRE
        }
        
        counter++;

        pub_led0->publish(message);

        if(counter >= 20) { // Après 1 seconde (20 * 50ms)
            counter = 0; // Réinitialiser le compteur
        }
    }

    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_led0;
    rclcpp::TimerBase::SharedPtr timer;
    int counter = 0;
    auto message = std_msgs::msg::Int32();
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<EPunkController>());
    rclcpp::shutdown();
    return 0;
}