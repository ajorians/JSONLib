#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include <map>

enum JSONType
{
   JSONType_Null,
   JSONType_String,
   JSONType_Bool,
   JSONType_Number,
   JSONType_Array,
   JSONType_Object
};

class JSON;
typedef std::map<std::string, JSON*> JSONObjects;
typedef std::vector<JSON*> JSONArray;

class JSON
{
public:
   static JSON* Parse(const std::string& strJSON);
   static JSON* Parse(const char **data);

   bool HasChild(const char* pstrName) const;
   JSON* Child(const char* pstrName);
   JSON* operator[](const char* pstr);

   std::size_t ElementCount() const;
   JSON* Element(std::size_t index);
   JSON* operator[](std::size_t index);

   std::string Stringify() const;

protected:
   JSON();
   JSON(const std::string& strJSON);
   JSON(const char* pstrJSON);
   JSON(bool b);
   JSON(double d);
   JSON(const std::vector<JSON*>& apValues);
   JSON(const std::map<std::string, JSON*>& mapValues);

public:
   bool IsNull() const;
   bool IsString() const;
   bool IsBool() const;
   bool IsNumber() const;
   bool IsArray() const;
   bool IsObject() const;

   const std::string& AsString() const;
   bool AsBool() const;
   double AsNumber() const;
   const JSONArray& AsArray() const;
   const JSONObjects& AsObject() const;

public:
   ~JSON();

protected:
   JSONType m_eType;
   std::string m_strValue;
   bool m_bValue;
   double m_dValue;
   JSONObjects m_mapElements;
   JSONArray m_apItems;
};

#endif
