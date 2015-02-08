#include "Test_JSON.h"

TEST(JSONTest, TestSimpleConstruction) {
   std::string strInput = "{\
                          \"firstName\": \"John\",\
                          \"lastName\": \"Smith\"}";

   JSON* pJSON = JSON::Parse(strInput);
   ASSERT_TRUE(pJSON != NULL);
   ASSERT_EQ(true, pJSON->HasChild("firstName"));
   ASSERT_EQ(true, pJSON->HasChild("lastName"));
   delete pJSON;
}

TEST(JSONTest, TestHarderConstruction) {
   std::string strInput = "{\
                          \"firstName\": \"John\",\
                          \"lastName\": \"Smith\"} 0";

   JSON* pJSON = JSON::Parse(strInput);
   ASSERT_TRUE(pJSON != NULL);
   ASSERT_EQ(true, pJSON->HasChild("firstName"));
   ASSERT_EQ(true, pJSON->HasChild("lastName"));
   ASSERT_EQ(true, pJSON->Child("firstName")->IsString());
   ASSERT_EQ("John", pJSON->Child("firstName")->AsString());
   delete pJSON;
}

TEST(JSONTest, TestConstruction) {
   std::string strInput = "{\
      \"firstName\": \"John\",\
      \"lastName\": \"Smith\",\
      \"isAlive\": true,\
      \"age\": 25,\
      \"height_cm\": 167.6,\
      \"address\": {\
         \"streetAddress\": \"21 2nd Street\",\
            \"city\": \"New York\",\
            \"state\": \"NY\",\
            \"postalCode\": \"10021-3100\"\
   },\
   \"phoneNumbers\": [\
         {\
            \"type\": \"home\",\
               \"number\": \"212 555-1234\"\
         },\
         {\
            \"type\": \"office\",\
               \"number\": \"646 555-4567\"\
            }\
   ],\
      \"children\": [],\
      \"spouse\": null\
   }";

   JSON* pJSON = JSON::Parse(strInput);
   ASSERT_TRUE(pJSON != NULL);
   ASSERT_EQ(true, pJSON->HasChild("firstName"));
   ASSERT_EQ(true, pJSON->HasChild("lastName"));
   ASSERT_EQ(true, pJSON->HasChild("isAlive"));
   ASSERT_EQ(true, pJSON->HasChild("age"));
   ASSERT_EQ(true, pJSON->HasChild("height_cm"));
   ASSERT_EQ(true, pJSON->HasChild("address"));
   ASSERT_EQ(true, pJSON->HasChild("phoneNumbers"));
   ASSERT_EQ(true, pJSON->HasChild("children"));
   ASSERT_EQ(true, pJSON->HasChild("spouse"));
   delete pJSON;
}

TEST(JSONTest, TestEscapeCharacters) {
   std::string strInput = "{\
                          \"firstName\": \"Jo\\hn\",\
                          \"lastName\": \"Smi\\th\"}";

   JSON* pJSON = JSON::Parse(strInput);

   std::string str = pJSON->Stringify();
   delete pJSON;
}

TEST(JSONTest, TestFlowResponse) {
   std::string strInput = "{\"event\":\"activity.user\",\"tags\":[],\"uuid\":null,\"persist\":false,\"id\":21947,\"flow\":\"9369c27b-da23-4139-9c21-75ca2e3e6f6c\",\"content\":{\"last_activity\":1423436879115},\"sent\":1423436968247,\"app\":null,\"created_at\":\"2015-02-08T23:09:28.247Z\",\"attachments\":[],\"user\":\"107463\"}";

   JSON* pJSON = JSON::Parse(strInput);
   ASSERT_TRUE(pJSON != NULL);
   delete pJSON;
}

TEST(JSONTest, TestFlowResponse2) {
   std::string strInput = "{\"event\":\"message\",\"tags\":[\":url\"],\"uuid\":\"qrxZtRh5mf_-UKRD\",\"id\":21946,\"flow\":\"9369c27b-da23-4139-9c21-75ca2e3e6f6c\",\"content\":\"Win10 news,  the office preview apps are available in the beta Store in Win 10,  just search for \"excel preview\", \"word preview\", or \"powerpoint preview\" : http:\\/\\/screencast.com\\/t\\/uuSYQTsCk more text\",\"sent\":1423436950953,\"app\":\"chat\",\"created_at\":\"2015-02-08T23:09:10.953Z\",\"attachments\":[],\"user\":\"107463\"}";

   JSON* pJSON = JSON::Parse(strInput);
   ASSERT_TRUE(pJSON != NULL);
   delete pJSON;
}

TEST(JSONTest, TestConstructing) {
   JSONObjects obj;
   obj["firstName"] = new JSON("John");
   obj["lastName"] = new JSON("Smith");
   int nValue = 5;
   obj["lastName"] = new JSON(nValue);
   JSON node(obj);
}

TEST(JSONTest, TestConst) {
   std::string strInput = "{\"event\":\"comment\",\"content\":{\"title\":\"test\",\"text\":\"hello\"}}";
   JSON* pJSON = JSON::Parse(strInput);
   ASSERT_TRUE(pJSON != NULL);
   const JSON* pContent = pJSON->Child("content");
   ASSERT_TRUE(pContent != NULL);
   JSON jContent(pContent->AsObject());
   JSON* pText = jContent["text"];
   ASSERT_TRUE(pText != NULL);
   ASSERT_TRUE(pText->IsString());
}

int main(int argc, char** argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   int nRet = RUN_ALL_TESTS();

#ifdef _DEBUG
   std::cout << "\nPress enter to continue...";
   std::cin.get();
#endif

   return nRet;
}
