// Currently we have to include the .cpp files or change DemoServer's configuration type 
// to DLL. We should probably look into another solution later.
#include "gtest/gtest.h"

#include "Tests/Utils/AckBufferTest.h"
#include "Tests/Game/Packet/OutPacketTest.h"


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
