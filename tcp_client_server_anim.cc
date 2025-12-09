#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TcpClientServerAnim");

int main(int argc, char *argv[])
{
    // Enable logging
    LogComponentEnable("PacketSink", LOG_LEVEL_INFO);
    LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);

    // Create 2 nodes
    NodeContainer nodes;
    nodes.Create(2);

    // Configure point-to-point link
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    // Install network devices
    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);

    // Install Internet stack
    InternetStackHelper stack;
    stack.Install(nodes);

    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    // Create TCP sink (server)
    uint16_t port = 5566;
    Address sinkAddress(InetSocketAddress(interfaces.GetAddress(1), port));
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
    ApplicationContainer sinkApp = packetSinkHelper.Install(nodes.Get(1));
    sinkApp.Start(Seconds(1.0));
    sinkApp.Stop(Seconds(10.0));

    // Create TCP client (OnOff app)
    OnOffHelper client("ns3::TcpSocketFactory", sinkAddress);
    client.SetAttribute("DataRate", StringValue("1Mbps"));
    client.SetAttribute("PacketSize", UintegerValue(1024));
    client.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    client.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer clientApp = client.Install(nodes.Get(0));
    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(9.0));

    // Enable packet tracing (optional)
    pointToPoint.EnablePcapAll("tcp_client_server_anim");

    // NetAnim setup
    AnimationInterface anim("tcp_client_server_anim.xml");
    anim.SetConstantPosition(nodes.Get(0), 10, 20);
    anim.SetConstantPosition(nodes.Get(1), 50, 20);

    // Run simulation
    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
