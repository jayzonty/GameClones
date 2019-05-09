#include "JSON.h"

#include <cstdio>

namespace JSON {
	namespace Detail {

		JSONTokenizer::JSONTokenizer(const char* jsonFilePath) : m_fileStream(jsonFilePath) {
		}

		JSONTokenizer::JSONTokenizer(const std::string& jsonFilePath) : JSONTokenizer(jsonFilePath.c_str()) {

		}

		JSONTokenizer::~JSONTokenizer() {
			if (m_fileStream.is_open()) {
				m_fileStream.close();
			}
			m_pendingTokens.clear();
		}

		bool JSONTokenizer::PeekNextToken(JSONToken& token) {
			if (HasNextToken()) {
				token.tokenType = m_pendingTokens.front().tokenType;
				token.literal = m_pendingTokens.front().literal;
				return true;
			}

			return false;
		}

		bool JSONTokenizer::PopNextToken(JSONToken& token) {
			if (HasNextToken()) {
				PeekNextToken(token);
				m_pendingTokens.pop_front();
				return true;
			}

			return false;
		}

		bool JSONTokenizer::HasNextToken() {
			if (m_pendingTokens.empty()) {
				return (ReadTokens() > 0);
			}

			return true;
		}

		void JSONTokenizer::DiscardNextToken() {
			if (!m_pendingTokens.empty()) {
				m_pendingTokens.pop_front();
			}
		}

		std::string ToLower(std::string& str) {
			std::string ret;
			for (int i = 0; i < str.length(); ++i) {
				ret += tolower(str[i]);
			}
			return ret;
		}

		int JSONTokenizer::ReadTokens() {
			int ret = 0;

			bool shouldEscape = false;
			char ch;
			while (m_fileStream.get(ch)) {
				if (!m_pendingTokens.empty() && (m_pendingTokens.back().tokenType == JSONTokenType::STRING)) {
					if (ch == '\"') {
						if (shouldEscape) {
							m_pendingTokens.back().literal += ch;
							shouldEscape = false;
						}
						else {
							m_pendingTokens.push_back({ JSONTokenType::STRING_END, "\"" });
							++ret;
							break;
						}
					}
					else {
						m_pendingTokens.back().literal += ch;
					}
				}
				else if (ch == '{') {
					m_pendingTokens.push_back({ JSONTokenType::OBJECT_START, "{" });
					++ret;
					break;
				}
				else if (ch == '}') {
					m_pendingTokens.push_back({ JSONTokenType::OBJECT_END, "}" });
					++ret;
					break;
				}
				else if (ch == '[') {
					m_pendingTokens.push_back({ JSONTokenType::ARRAY_START, "[" });
					++ret;
					break;
				}
				else if (ch == ']') {
					m_pendingTokens.push_back({ JSONTokenType::ARRAY_END, "]" });
					++ret;
					break;
				}
				else if (ch == ',') {
					m_pendingTokens.push_back({ JSONTokenType::ITEM_SEPARATOR, "," });
					++ret;
					break;
				}
				else if (ch == ':') {
					m_pendingTokens.push_back({ JSONTokenType::KEY_VALUE_SEPARATOR, ":" });
					++ret;
					break;
				}
				else if (ch == '-' && (m_pendingTokens.empty() || m_pendingTokens.back().tokenType != JSONTokenType::NUMBER)) {
					m_pendingTokens.push_back({ JSONTokenType::NUMBER, "-" });
				}
				else if (ch == '\"') {
					if (m_pendingTokens.empty() || m_pendingTokens.back().tokenType != JSONTokenType::STRING) {
						m_pendingTokens.push_back({ JSONTokenType::STRING_START, "\"" });
						m_pendingTokens.push_back({ JSONTokenType::STRING, "" });
						ret += 2;
					}
				}
				else if (isdigit(ch)) {
					if (m_pendingTokens.empty() || m_pendingTokens.back().tokenType != JSONTokenType::NUMBER) {
						m_pendingTokens.push_back({ JSONTokenType::NUMBER, "" });
					}
					m_pendingTokens.back().literal += ch;
				}
				else if (!m_pendingTokens.empty() && (m_pendingTokens.back().tokenType == JSONTokenType::NUMBER) && (ch == 'e' || ch == '.')) {
					m_pendingTokens.back().literal += ch;
				}
				else if (!iswspace(ch)) {
					if (m_pendingTokens.empty() || (m_pendingTokens.back().tokenType != JSONTokenType::UNDEFINED)) {
						m_pendingTokens.push_back({ JSONTokenType::UNDEFINED, "" });
					}
					m_pendingTokens.back().literal += ch;

					std::string lowerStr = ToLower(m_pendingTokens.back().literal);
					if ((lowerStr == "true") || (lowerStr == "false")) {
						m_pendingTokens.back().literal = lowerStr;
						m_pendingTokens.back().tokenType = JSONTokenType::BOOLEAN;
					}
				}
			}

			return ret;
		}

		std::string JSONTokenTypeToString(JSONTokenType tokenType) {
			if (tokenType == JSONTokenType::OBJECT_START) {
				return "OBJECT_START";
			}
			else if (tokenType == JSONTokenType::OBJECT_END) {
				return "OBJECT_END";
			}
			else if (tokenType == JSONTokenType::STRING_START) {
				return "STRING_START";
			}
			else if (tokenType == JSONTokenType::STRING) {
				return "STRING";
			}
			else if (tokenType == JSONTokenType::STRING_END) {
				return "STRING_END";
			}
			else if (tokenType == JSONTokenType::ARRAY_START) {
				return "ARRAY_START";
			}
			else if (tokenType == JSONTokenType::ARRAY_END) {
				return "ARRAY_END";
			}
			else if (tokenType == JSONTokenType::ITEM_SEPARATOR) {
				return "ITEM_SEPARATOR";
			}
			else if (tokenType == JSONTokenType::KEY_VALUE_SEPARATOR) {
				return "KEY_VALUE_SEPARATOR";
			}
			else if (tokenType == JSONTokenType::NUMBER) {
				return "NUMBER";
			}

			return "UNIDENTIFIED";
		}
	}

	JSONNode::JSONNode() {
	}

	JSONNode::~JSONNode() {
	}

	std::string JSONNode::ToString() {
		return ToIndentedString(0);
	}

	JSONString::JSONString(const std::string& str) : JSONNode(), m_string(str) {
	}

	const std::string& JSONString::GetString() {
		return m_string;
	}

	std::string JSONString::ToIndentedString(int level) {
		return m_string;
	}

	JSONArray::JSONArray() : JSONNode() {
	}

	JSONArray::~JSONArray() {
		for (int i = 0; i < m_items.size(); ++i) {
			delete m_items[i];
		}
		m_items.clear();
	}

	void JSONArray::AddNode(JSONNode* node) {
		m_items.push_back(node);
	}

	JSONNode* JSONArray::Get(int index) {
		return m_items[index];
	}

	size_t JSONArray::Size() const {
		return m_items.size();
	}

	std::string JSONArray::ToIndentedString(int level) {
		std::string ret = GetIndentString(level) + "[\n";
		for (int i = 0; i < m_items.size(); ++i) {
			ret += GetIndentString(level + 1) + m_items[i]->ToIndentedString(level + 1) + ",\n";
		}
		ret += GetIndentString(level) + "]";
		return ret;
	}

	JSONObject::JSONObject() : JSONNode() {
	}

	JSONObject::~JSONObject() {
		for (auto& pair : m_keyValuePairs) {
			delete pair.second;
		}
		m_keyValuePairs.clear();
	}

	void JSONObject::AddNode(std::string key, JSONNode* val) {
		if (m_keyValuePairs.find(key) == m_keyValuePairs.end()) {
			m_keyValuePairs.insert({ key, val });
		}
		else {
			delete m_keyValuePairs[key];
			m_keyValuePairs[key] = val;
		}
	}

	void JSONObject::AddNode(const char* key, JSONNode* val) {
		std::string temp(key);
		AddNode(temp, val);
	}

	JSONNode* JSONObject::Get(std::string key) {
		if (m_keyValuePairs.find(key) == m_keyValuePairs.end()) {
			return nullptr;
		}

		return m_keyValuePairs[key];
	}

	JSONNode* JSONObject::Get(const char* key) {
		return Get(std::string(key));
	}

	std::string JSONObject::ToIndentedString(int level) {
		std::string ret = GetIndentString(level) + "{\n";
		for (auto& pair : m_keyValuePairs) {
			ret += GetIndentString(level + 1);
			ret += "\"" + pair.first + "\": ";
			ret += pair.second->ToIndentedString(level + 1);
			ret += ",\n";
		}
		ret += GetIndentString(level) + "}";

		return ret;
	}

	JSONNumber::JSONNumber(std::string& originalString) : m_originalString(originalString) {
	}

	JSONNumber::~JSONNumber() {
	}

	int32_t JSONNumber::AsInt32() const {
		return std::stoi(m_originalString);
	}

	float JSONNumber::AsFloat() const {
		return std::stof(m_originalString);
	}

	double JSONNumber::AsDouble() const {
		return std::stod(m_originalString);
	}

	std::string JSONNumber::ToIndentedString(int level) {
		return m_originalString;
	}

	JSONBoolean::JSONBoolean(bool value) : m_value(value) {
	}

	JSONBoolean::~JSONBoolean() {
	}

	std::string JSONBoolean::ToIndentedString(int level) {
		std::string ret = m_value ? "true" : "false";
		return ret;
	}

	JSONParser::JSONParser(const char* jsonFilePath) : m_tokenizer(jsonFilePath), m_root(nullptr) {
	}

	JSONParser::JSONParser(const std::string& jsonFilePath) : JSONParser(jsonFilePath.c_str()) {
	}

	JSONParser::~JSONParser() {
		delete m_root;
	}

	JSONObject* JSONParser::Parse() {
		if (m_root == nullptr) {
			m_root = ParseObject();
		}

		return m_root;
	}

	JSONNode* JSONParser::ParseNode() {
		Detail::JSONToken token;
		while (m_tokenizer.PeekNextToken(token)) {
			if (token.tokenType == Detail::JSONTokenType::STRING_START) {
				return ParseString();
			}
			else if (token.tokenType == Detail::JSONTokenType::OBJECT_START) {
				return ParseObject();
			}
			else if (token.tokenType == Detail::JSONTokenType::ARRAY_START) {
				return ParseArray();
			}
			else if (token.tokenType == Detail::JSONTokenType::NUMBER) {
				m_tokenizer.DiscardNextToken();
				return new JSONNumber(token.literal);
			}
			else if (token.tokenType == Detail::JSONTokenType::BOOLEAN) {
				m_tokenizer.DiscardNextToken();
				return new JSONBoolean(token.literal == "true");
			}
		}
	}

	JSONString* JSONParser::ParseString() {
		Detail::JSONToken token;
		if (!m_tokenizer.PeekNextToken(token)) {
			return nullptr;
		}
		if (token.tokenType != Detail::JSONTokenType::STRING_START) {
			return nullptr;
		}
		m_tokenizer.DiscardNextToken();

		m_tokenizer.PopNextToken(token);

		JSONString* ret = new JSONString(token.literal);

		m_tokenizer.DiscardNextToken(); // STRING_END

		return ret;
	}

	JSONObject* JSONParser::ParseObject() {
		Detail::JSONToken token;
		if (!m_tokenizer.PeekNextToken(token)) {
			return nullptr;
		}
		if (token.tokenType != Detail::JSONTokenType::OBJECT_START) {
			return nullptr;
		}
		m_tokenizer.DiscardNextToken();

		JSONObject* ret = new JSONObject();

		std::string key;
		while (m_tokenizer.PopNextToken(token)) {
			if (token.tokenType == Detail::JSONTokenType::OBJECT_END) {
				break;
			}

			if (token.tokenType == Detail::JSONTokenType::KEY_VALUE_SEPARATOR) {
				JSONNode* obj = ParseNode();

				ret->AddNode(key, obj);
			}
			else if (token.tokenType == Detail::JSONTokenType::STRING_START) {
				m_tokenizer.PopNextToken(token);
				key = token.literal;
				m_tokenizer.DiscardNextToken();
			}
		}

		return ret;
	}

	JSONArray* JSONParser::ParseArray() {
		Detail::JSONToken token;
		if (!m_tokenizer.PeekNextToken(token)) {
			return nullptr;
		}
		if (token.tokenType != Detail::JSONTokenType::ARRAY_START) {
			return nullptr;
		}
		m_tokenizer.DiscardNextToken();

		JSONArray* ret = new JSONArray();
		while (m_tokenizer.PeekNextToken(token)) {
			if (token.tokenType == Detail::JSONTokenType::ARRAY_END) {
				m_tokenizer.DiscardNextToken();
				break;
			}

			if (token.tokenType != Detail::JSONTokenType::ITEM_SEPARATOR) {
				JSONNode* temp = ParseNode();
				ret->AddNode(temp);
			}
			else {
				m_tokenizer.DiscardNextToken();
			}
		}

		return ret;
	}

	JSONObject* AsObject(JSONNode* node) {
		return dynamic_cast<JSONObject*>(node);
	}

	JSONArray* AsArray(JSONNode* node) {
		return dynamic_cast<JSONArray*>(node);
	}

	JSONBoolean* AsBoolean(JSONNode* node) {
		return dynamic_cast<JSONBoolean*>(node);
	}

	JSONNumber* AsNumber(JSONNode* node) {
		return dynamic_cast<JSONNumber*>(node);
	}

	JSONString* AsString(JSONNode* node) {
		return dynamic_cast<JSONString*>(node);
	}
}