#include "ros/ros.h"
#include "order_publisher/dummy_msg_creator.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "order_publisher");
    ros::NodeHandle n;

    // initialize a publisher
    ros::Publisher order_pub = n.advertise<vda5050_msgs::Order>("order", 1000);

    // read parameters from server
    float new_order_freq;
    float order_update_freq;
    int num_of_nodes;
    int num_of_released_nodes;


    n.getParam("order_publisher/new_order_frequency", new_order_freq);
    ROS_INFO("New order is published every %fs", new_order_freq);
    n.getParam("order_publisher/order_update_frequency", order_update_freq);
    ROS_INFO("Order update is published every %fs", order_update_freq);
    n.getParam("order_publisher/num_of_nodes", num_of_nodes);
    ROS_INFO("The number of nodes every order is %d", num_of_nodes);
    n.getParam("order_publisher/num_of_released_nodes", num_of_released_nodes);
    ROS_INFO("The number of released nodes every order is %d", num_of_released_nodes);

    // set publish frequency
    auto freq = 1 / order_update_freq;
    int divisor = new_order_freq / order_update_freq;

    ros::Rate loop_rate(freq);
    int count = 0;

    // initialize the chatacteristics of publisher

    OrderMsg *OrderMsgPtr = new OrderMsg();
    int new_headerId = 0;
    std::string new_orderId = OrderMsgPtr->rand_str(36);
    int new_oderUpdateId = 0;
    int node_num = num_of_nodes;
    int node_released_num = num_of_released_nodes;
    
    // publish the msgs
    while (ros::ok())
    {
        
        OrderMsgPtr->create_example_order(new_headerId,
                                          new_orderId,
                                          new_oderUpdateId,
                                          node_num,
                                          node_released_num);

        order_pub.publish(OrderMsgPtr->get_msg());

        count++;
        new_oderUpdateId++;

        if (count % divisor == 0)
        {
            new_headerId++;
            new_orderId = OrderMsgPtr->rand_str(36);
            new_oderUpdateId = 0;
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    delete OrderMsgPtr;
    return 0;
}
