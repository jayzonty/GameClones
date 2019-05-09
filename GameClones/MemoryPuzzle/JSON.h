#pragma once

#ifndef ___JSON_h___
#define ___JSON_h___

#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace JSON {
	namespace Detail {
		enum class JSONTokenType {
			UNDEFINED, OBJECT_START, OBJECT_END, STRING_START, STRING, STRING_END,
			ARRAY_START, ARRAY_END, KEY_VALUE_SEPARATOR, ITEM_SEPARATOR,
			NUMBER, BOOLEAN
		};

		struct JSONToken {
			JSONTokenType tokenType;
			std::string literal;
		};

		class JSONTokenizer {
			std::list<JSONToken> m_pendingTokens;
			std::ifstream m_fileStream;

		public:
			JSONTokenizer(const char* jsonFilePath);
			JSONTokenizer(const std::string& jsonFilePath);
			~JSONTokenizer();

			bool PeekNextToken(JSONToken& token);
			bool PopNextToken(JSONToken& token);
			bool HasNextToken();

			void DiscardNextToken();

		private:
			int ReadTokens();
		};

		std::string JSONTokenTypeToString(JSONTokenType tokenType);
	}

	class JSONNode {
	public:
		JSONNode();
		virtual ~JSONNode() = 0;

		std::string ToString();

		virtual std::string ToIndentedString(int level) = 0;

	protected:
		virtual std::string GetIndentString(int level) {
			std::string ret;
			for (int i = 0; i < level; ++i) {
				ret += "\t";
			}
			return ret;
		}
	};

	class JSONString : public JSONNode {
		std::string m_string;

	public:
		JSONString(const std::string& str);

		const std::string& GetString();

		std::string ToIndentedString(int level) override;
	};

	class JSONArray : public JSONNode {
		std::vector<JSONNode*> m_items;

	public:
		JSONArray();
		~JSONArray();

		void AddNode(JSONNode* node);
		JSONNode* Get(int index);

		size_t Size() const;

		std::string ToIndentedString(int level) override;
	};

	class JSONObject : public JSONNode {
		std::unordered_map<std::string, JSONNode*> m_keyValuePairs;

	public:
		JSONObject();
		~JSONObject();

		void AddNode(std::string key, JSONNode* val);
		void AddNode(const char* key, JSONNode* val);

		JSONNode* Get(std::string key);
		JSONNode* Get(const char* key);

		std::string ToIndentedString(int level) override;
	};

	class JSONNumber : public JSONNode {
		std::string m_originalString;
	public:
		JSONNumber(std::string& originalString);
		~JSONNumber();

		int32_t AsInt32() const;
		float AsFloat() const;
		double AsDouble() const;

		std::string ToIndentedString(int level) override;
	};

	class JSONBoolean : public JSONNode {
		bool m_value;
	public:
		JSONBoolean(bool value);
		~JSONBoolean();

		std::string ToIndentedString(int level) override;
	};

	class JSONParser {
		Detail::JSONTokenizer m_tokenizer;

		JSONObject* m_root;
	public:
		JSONParser(const char* jsonFilePath);
		JSONParser(const std::string& jsonFilePath);
		~JSONParser();

		JSONObject* Parse();

	private:
		JSONNode* ParseNode();
		JSONString* ParseString();
		JSONObject* ParseObject();
		JSONArray* ParseArray();
	};

	JSONObject* AsObject(JSONNode* node);
	JSONArray* AsArray(JSONNode* node);
	JSONBoolean* AsBoolean(JSONNode* node);
	JSONNumber* AsNumber(JSONNode* node);
	JSONString* AsString(JSONNode* node);
}

#endif // ___JSON_h___
