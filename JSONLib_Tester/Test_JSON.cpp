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
