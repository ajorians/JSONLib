#include "JSON.h"
#include <cassert>

namespace {
   bool SkipWhitespace(const char **data)
   {
      while (**data != 0 && (**data == ' ' || **data == '\t' || **data == '\r' || **data == '\n'))
         (*data)++;

      return **data != 0;
   }

   bool ExtractString(const char **data, std::string &str)
   {
      str.clear();

      while( **data != 0 ) {
         char chNext = **data;

         if( chNext == '"' ) {
            (*data)++;
            str.reserve();
            return true;
         }

         str += chNext;

         (*data)++;
      }

      return false;
   }

   static inline bool special_strlen(const char *s, size_t n) {
      if( s == 0 )
         return false;

      const char *save = s;
      while( n--> 0 )
      {
         if (*(save++) == 0) return false;
      }

      return true;
   }

   static inline int special_strcmpi(const char *s1, const char *s2, size_t n)
   {
      int lc1  = 0;
      int lc2  = 0;

      while (n--)
      {
         lc1 = tolower(*s1);
         lc2 = tolower(*s2);

         if (lc1 != lc2)
            return (lc1 - lc2);

         if (!lc1)
            return 0;

         ++s1;
         ++s2;
      }

      return 0;
   }

   double ParseInt(const char **data)
   {
      double integer = 0;
      while (**data != 0 && **data >= '0' && **data <= '9')
         integer = integer * 10 + (*(*data)++ - '0');

      return integer;
   }

   double ParseDecimal(const char **data)
   {
      double decimal = 0.0;
      double factor = 0.1;
      while (**data != 0 && **data >= '0' && **data <= '9')
      {
         int digit = (*(*data)++ - '0');
         decimal = decimal + digit * factor;
         factor *= 0.1;
      }
      return decimal;
   }
}

JSON::JSON()
{
   m_eType = JSONType_Null;
}

JSON::JSON(const std::string& strJSON)
{
   m_eType = JSONType_String;
   m_strValue = strJSON;
}

JSON::JSON(const char* pstrJSON)
{
   m_eType = JSONType_String;
   m_strValue = std::string(pstrJSON);
}

JSON::JSON(bool b)
{
   m_eType = JSONType_Bool;
   m_bValue = b;
}

JSON::JSON(double d)
{
   m_eType = JSONType_Number;
   m_dValue = d;
}

JSON::JSON(const std::vector<JSON*>& apValues)
{
   m_eType = JSONType_Array;
   m_apItems = apValues;
}

JSON::JSON(const std::map<std::string, JSON*>& mapValues)
{
   m_eType = JSONType_Object;
   m_mapElements = mapValues;
}

bool JSON::IsNull() const { return m_eType == JSONType_Null; }
bool JSON::IsString() const { return m_eType == JSONType_String; }
bool JSON::IsBool() const { return m_eType == JSONType_Bool; }
bool JSON::IsNumber() const{ return m_eType == JSONType_Number; }
bool JSON::IsArray() const{ return m_eType == JSONType_Array; }
bool JSON::IsObject() const{ return m_eType == JSONType_Object; }

const std::string& JSON::AsString() const { assert(IsString()); return m_strValue; }
bool JSON::AsBool() const { assert(IsBool()); return m_bValue; }
double JSON::AsNumber() const { assert(IsNumber()); return m_dValue; }
const JSONArray& JSON::AsArray() const { assert(IsArray()); return m_apItems; }
const JSONObjects& JSON::AsObject() const { assert(IsObject()); return m_mapElements; }

JSON::~JSON()
{
   //Delete member vars
   JSONObjects::iterator iter;
   for (iter = m_mapElements.begin(); iter != m_mapElements.end(); iter++)
      delete (*iter).second;
}

JSON* JSON::Parse(const std::string& strJSON)
{
   const char* pstr = strJSON.c_str();

   return JSON::Parse(&pstr);
}

JSON* JSON::Parse(const char **data)
{
   SkipWhitespace(data);

   if( **data == '"' ) {
      std::string str;
      if( !ExtractString(&(++(*data)), str) ) {
         goto Failed;
      }
      return new JSON(str);
   }

   else if( (special_strlen(*data, 4) && special_strcmpi(*data, "true", 4)==0) || (special_strlen(*data, 5) && special_strcmpi(*data, "false", 5)==0) ) {
      bool b = special_strlen(*data, 4) && special_strcmpi(*data, "true", 4)==0;
      (*data) += b ? 4 : 5;
      return new JSON(b);
   }

   else if( special_strlen(*data, 4) && special_strcmpi(*data, "null", 4)==0 ) {
      (*data) += 4;
      return new JSON();
   }

   else if( **data == '-' || (**data >= '0' && **data <= '9') ) {
      bool neg = **data == L'-';
      if (neg) (*data)++;

      double number = 0.0;

      if (**data == '0')
         (*data)++;
      else if (**data >= '1' && **data <= '9')
         number = ParseInt(data);
      else
         goto FailureNumber;

      if (**data == '.')
      {
         (*data)++;

         if (!(**data >= '0' && **data <= '9'))
            goto FailureNumber;

         double decimal = ParseDecimal(data);

         number += decimal;
      }

      if (**data == 'E' || **data == 'e')
      {
         (*data)++;

         bool neg_expo = false;
         if (**data == '-' || **data == '+')
         {
            neg_expo = **data == '-';
            (*data)++;
         }

         if (!(**data >= '0' && **data <= '9'))
            goto FailureNumber;

         double expo = ParseInt(data);
         for (double i = 0.0; i < expo; i++)
            number = neg_expo ? (number / 10.0) : (number * 10.0);
      }

      if (neg) number *= -1;

      return new JSON(number);

FailureNumber:
      goto Failed;
   }

   else if( **data == '{' ) {
      JSONObjects mapValues;
      (*data)++;

      while (**data != 0)
      {
         if( !SkipWhitespace(data) ) {
            goto FailureObject;
         }

         if( **data == '}' ) {
            (*data)++;
            return new JSON(mapValues);
         }

         std::string strName;
         if( !ExtractString(&(++(*data)), strName) ) {
            goto FailureObject;
         }

         if( !SkipWhitespace(data) ) {
            goto FailureObject;
         }

         if( *((*data)++) != ':' ) {
            goto FailureObject;
         }

         if( !SkipWhitespace(data) ) {
            goto FailureObject;
         }

         JSON* pValue = JSON::Parse(data);
         if( pValue == NULL ) {
            goto FailureObject;
         }

         if( mapValues.find(strName) != mapValues.end() )
            delete mapValues[strName];
         mapValues[strName] = pValue;

         if( !SkipWhitespace(data) ) {
            goto FailureObject;
         }

         if( **data == '}' )
         {
            (*data)++;
            return new JSON(mapValues);
         }

         if( *((*data)++) != ',' ) {
            goto FailureObject;
         }

         (*data)++;
      }

FailureObject:
      JSONObjects::iterator it;
      for(it = mapValues.begin(); it != mapValues.end(); it++) { delete it->second; }
      goto Failed;
   }

   else if (**data == L'[')
   {
      std::vector<JSON*> apValues;

      (*data)++;

      while (**data != 0)
      {
         if (!SkipWhitespace(data)) {
            goto FailureArray;
         }

         if (apValues.size() == 0 && **data == ']') {
            (*data)++;
            return new JSON(apValues);
         }

         JSON *pValue = Parse(data);
         if (pValue == NULL) {
            goto FailureArray;
         }

         apValues.push_back(pValue);

         if (!SkipWhitespace(data)) {
            goto FailureArray;
         }

         if (**data == ']') {
            (*data)++;
            return new JSON(apValues);
         }

         if (**data != ',') {
            goto FailureArray;
         }

         (*data)++;
      }

FailureArray:
      std::vector<JSON*>::iterator it;
      for (it = apValues.begin(); it != apValues.end(); it++) { delete *it; }
      goto Failed;
   }

Failed:
   return NULL;
}

bool JSON::HasChild(const char* pstrName) const
{
   if( m_eType == JSONType_Object ) {
      return m_mapElements.find(pstrName) != m_mapElements.end();
   }
   return false;
}

JSON* JSON::Child(const char* pstrName)
{
   JSONObjects::iterator it = m_mapElements.find(pstrName);
   if( it != m_mapElements.end() )
      return it->second;
   return NULL;
}

JSON* JSON::operator[](const char* pstr)
{
   return Child(pstr);
}

std::size_t JSON::ElementCount() const
{
   if( m_eType == JSONType_Object )
      return m_apItems.size();
   return 0;
}

JSON* JSON::Element(std::size_t index)
{
   if( index < m_apItems.size() )
      return m_apItems[index];
   return NULL;
}

JSON* JSON::operator[](std::size_t index)
{
   return Element(index);
}
