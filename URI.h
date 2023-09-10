#ifndef ___Q_URI_H___
#define ___Q_URI_H___
#include <string>
#include <vector>
#include <sstream>
struct URI {
	std::string scheme, authority, path, query, fragment;
	std::string userInfo, host;
	unsigned short port = 0;
	std::vector<std::string> pathSegments, querySegments;
	static std::string percentageEncode(std::string str) {
		std::stringstream ss;
		for (auto c : str) {
			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
				ss << c;
			else
				ss << '%' << std::hex << (int)c;
		}
		return ss.str();
	}
	static std::string percentageDecode(std::string str) {
		std::stringstream ss;
		for (size_t i = 0; i < str.size(); ++i) {
			if (str[i] == '%') {
				ss << (char)std::stoi(str.substr(i + 1, 2), nullptr, 16);
				i += 2;
			}
			else
				ss << str[i];
		}
		return ss.str();
	}
	URI(std::string str) {
		size_t end = str.find(':');
		scheme = str.substr(0, end);
		size_t start = end + 1;
		if (str[start + 1] == '/') {
			start += 2;
			end = str.find('/', start);
			authority = str.substr(start, end - start);
			start = end + 1;
		}
		end = str.find('?', start);
		if (end != std::string::npos) {
			path = str.substr(start, end - start);
			start = end + 1;
			end = str.find('#', start);
			if (end != std::string::npos) {
				query = str.substr(start, end - start);
				fragment = str.substr(end + 1);
			}
			else
				query = str.substr(start);
		}
		else {
			end = str.find('#', start);
			if (end != std::string::npos) {
				path = str.substr(start, end - start);
				fragment = str.substr(end + 1);
			}
			else
				path = str.substr(start);
		}

		start = 0;
		end = authority.find('@');
		if (end != std::string::npos) {
			userInfo = authority.substr(0, end);
			start = end + 1;
		}
		end = authority.find_last_of(':');
		if (end != std::string::npos) {
			host = authority.substr(start, end - start);
			port = std::stoi(authority.substr(end + 1));
		}
		else
			host = authority.substr(start);

		start = 0;
		while ((end = path.find('/', start)) != std::string::npos) {
			pathSegments.push_back(path.substr(start, end - start));
			start = end + 1;
		}
		pathSegments.push_back(path.substr(start));
		if (pathSegments.back().empty())
			pathSegments.pop_back();
		pathSegments.shrink_to_fit();

		start = 0;
		while (((end = query.find('&', start)) != std::string::npos)) {
			querySegments.push_back(query.substr(start, end - start));
			start = end + 1;
		}
		querySegments.push_back(query.substr(start));
		if (querySegments.back().empty())
			querySegments.pop_back();
		querySegments.shrink_to_fit();
	}
};
#endif