#include <base/component.h>
#include <util/xml_node.h>
#include <os/config.h>
#include <nic/packet_allocator.h>
#include <base/log.h>
#include <lwip/genode.h>

#define pi 3.14f

extern "C" {
#include "extApi.h"
#include "extApiPlatform.h"
#include "v_repConst.h"
}

Genode::size_t Component::stack_size() { return 64*1024; }

void Component::construct(Genode::Env &)
{
	enum { BUF_SIZE = Nic::Packet_allocator::DEFAULT_PACKET_SIZE * 128 };

	//lwip_tcpip_init();

	Genode::Xml_node network = Genode::config()->xml_node().sub_node("network");

	if (network.attribute_value<bool>("dhcp", true)) {
		if (lwip_nic_init(0,
						  0,
						  0,
						  BUF_SIZE,
						  BUF_SIZE)) {
			return;
		}
	} else {
		char ip_addr[16] = { 0 };
		char subnet[16] = { 0 };
		char gateway[16] = { 0 };

		try {
			network.attribute("ip-address").value(ip_addr, sizeof(ip_addr));
			network.attribute("subnet").value(subnet, sizeof(subnet));
			network.attribute("gateway").value(gateway, sizeof(gateway));
		} catch(Genode::Xml_node::Nonexistent_attribute) {
			Genode::error("please check the network configuration");
			return;
		}
	}

	int portNb = 19997;
	int color = 0;
	// Genode::log("DEBUG");
	

	// if (argc >= 2)
	// {
	//     // portNb = atoi(argv[1]);
	//     // leftMotorHandle=atoi(argv[2]);
	//     // rightMotorHandle=atoi(argv[3]);
	//     // sensorHandle=atoi(argv[4]);
	// }
	// else
	// {
	//     Genode::log("Indicate following arguments: 'portNumber leftMotorHandle rightMotorHandle sensorHandle'!\n");
	//     // printf("Indicate following arguments: 'portNumber leftMotorHandle rightMotorHandle sensorHandle'!\n");
	//     extApi_sleepMs(5000);
	//     return 0;
	// }
	int retStringCnt;
	char* retStrings;

	int clientID;
	do {
		clientID = simxStart((simxChar*)"192.168.137.1", portNb, true, true, 2000, 5);
	} while(clientID != 0);
	Genode::log("connected!");

	if (clientID!=-1)
	{

		//int result=simxCallScriptFunction(clientID,"remoteApiCommandServer",sim_scripttype_childscript,"displayText_function",0,NULL,0,NULL,1,"Hello world!",0,NULL,NULL,NULL,NULL,NULL,&retStringCnt,&retStrings,NULL,NULL,simx_opmode_blocking);
		//simxSetIntegerSignal(clientID,"cubeColor",42,simx_opmode_blocking);
		//printf("Color set");
		//int result = simxCallScriptFunction(clientID,"IRB140",sim_scripttype_childscript,"test",0,NULL,0,NULL,0,NULL,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,simx_opmode_blocking);
		//extApi_sleepMs(5000);
		//int result = simxCallScriptFunction(clientID,"IRB140",sim_scripttype_childscript,"blueCube",0,NULL,0,NULL,0,NULL,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,simx_opmode_blocking);
		//printf("result: %d\n", result);
		int old_color = 0;
		//   simxGetIntegerSignal(clientID,"ccubeColor",&color,simx_opmode_streaming);
		//   printf("color: %d\n", color);
		//   old_color = color;
		//   printf("old_color %d\n", old_color);
		while (simxGetConnectionId(clientID)!=-1)
		{
			//printf("Color: %d\n", color);
			//int res,signalValue=simxGetIntegerSignal(clientID,"ccubeColor",&color,simx_opmode_streaming);
			simxGetIntegerSignal(clientID,"ccubeColor",&color,simx_opmode_streaming);
			//   if (color != 0)
			//   {
			//       printf("Color: %d\n", color);
			//       printf("Old_Color: %d\n", old_color);
			//   }
			simxSetIntegerSignal(clientID,"cubeColor",0,simx_opmode_blocking);
			if (old_color != color)
			{
				//printf("if first");
				old_color = color;
				//printf("Color: %d\n", color);
				//simxSetIntegerSignal(clientID,"cubeColor",0,simx_opmode_blocking);
				//printf("color: %d\n", color);
				if (color == 1)
				{
					//printf("if 1");
					simxSetIntegerSignal(clientID,"cubeColor",1,simx_opmode_blocking);
					Genode::log("Client: Received signal -> blue cube... sending signal to server\n");
					// simxSetIntegerSignal(clientID,"cubeColor",0,simx_opmode_blocking);
					//extApi_sleepMs(5000);
					continue;
					//color = 0;
				}
				else if (color == 2)
				{
					//printf("if 1");
					simxSetIntegerSignal(clientID,"cubeColor",2,simx_opmode_blocking);
					Genode::log("Client: Received signal -> red cube... sending signal to server\n");
					// simxSetIntegerSignal(clientID,"cubeColor",0,simx_opmode_blocking);
					//extApi_sleepMs(5000);
					continue;
					//color = 0;
				}
			}
			// else
			// {
			//   //printf("ERROR! COLOR NOT RECOGNIZED!");
			// }
		}
		simxFinish(clientID);
		// int driveBackStartTime=-99000;
		// float motorSpeeds[2];
		//
		// while (simxGetConnectionId(clientID)!=-1)
		// {
		//     simxUChar sensorTrigger=0;
		//     if (simxReadProximitySensor(clientID,sensorHandle,&sensorTrigger,NULL,NULL,NULL,simx_opmode_streaming)==simx_return_ok)
		//     { // We succeeded at reading the proximity sensor
		//         int simulationTime=simxGetLastCmdTime(clientID);
		//         if (simulationTime-driveBackStartTime<4000)
		//         { // driving backwards while slightly turning:
		//             motorSpeeds[0]=-pi*0.5f;
		//             motorSpeeds[1]=-pi*0.1f;
		//         }
		//         else
		//         { // going forward:
		//             motorSpeeds[0]=pi;
		//             motorSpeeds[1]=pi;
		//             if (sensorTrigger)
		//                 driveBackStartTime=simulationTime; // We detected something, and start the backward mode
		//         }
		//         simxSetJointTargetVelocity(clientID,leftMotorHandle,motorSpeeds[0],simx_opmode_oneshot);
		//         simxSetJointTargetVelocity(clientID,rightMotorHandle,motorSpeeds[1],simx_opmode_oneshot);
		//     }
		//     extApi_sleepMs(5);
		// }
		// simxFinish(clientID);
	}
}
