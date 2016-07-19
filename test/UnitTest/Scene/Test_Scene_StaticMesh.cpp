#include <TestConfig.h>


class Test_Scene_StaticMesh : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_StaticMesh, Basic)
{
	auto shader = Shader::Create(LN_LOCALFILE("../../../src/Graphics/"));
}
