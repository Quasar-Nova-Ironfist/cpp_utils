#ifndef QUASAR_URI_H
#define QUASAR_URI_H
/* For a demonstration of the URI struct run the following code:
	quasar::URI url("https://johnBlandguy:isurehope@chanty.cultists.com:666/test/path/of/doom?prev=4.01747446788347%2B31354386+ratio&tags=sacrificial_iguana%20order%3Apopular&page=12#fragmentTest");
	cout << "whole: " << url << endl;
	cout << "scheme: " << url.scheme << endl;
	cout << "authority: " << url.authority << endl;
	cout << "\tuserInfo: " << url.userInfo << endl;
	cout << "\thost: " << url.host << endl;
	cout << "\tport: " << url.port << endl;
	cout << "path: " << url.path << endl;
	cout << "pathSegments: " << endl;
	for (auto& s : url.pathSegments) {
		cout << '\t' << s << endl;
	}
	cout << "query: " << url.query << endl;
	cout << "querySegments: " << endl;
	for (auto& s : url.querySegments) {
		cout << '\t' << s << endl;
		cout << "\t\t" << s.key << endl;
		cout << "\t\t" << s.value << endl;
		for (auto& v : s.valueSegments)
			cout << "\t\t\t" << v << endl;
	}
	cout << "fragment: " << url.fragment << endl;
*/
#include <string>
#include <vector>
#include <sstream>
namespace quasar {
	struct URI {
		using string = std::string;
		struct querySegment;

		string whole, scheme, authority, path, query, fragment, userInfo, host;
		unsigned short port = 0;
		std::vector<string> pathSegments;
		std::vector<querySegment> querySegments;

		struct querySegment {
			string whole, key, value;
			std::vector<string> valueSegments;

			querySegment(string str);
			operator string() const;
			string operator()() const;
			string operator[](int i) const;
			operator string&();
			string& operator()();
			string& operator[](int i);
		};
		static string percentageEncode(string str);
		static string percentageDecode(string str);
		URI();//in case you just want the slots in which to place strings rather than parsing one
		URI(string str);//assumes passed string is a valid URI
		operator string() const;
		string operator()() const;
		operator string& ();
		string& operator()();
	};
}
std::ostream& operator<<(std::ostream& os, const quasar::URI::querySegment& q);
std::ostream& operator<<(std::ostream& os, const quasar::URI& u);

#define QUASAR_IMPLEMENTATION

#ifdef QUASAR_IMPLEMENTATION
quasar::URI::querySegment::querySegment(string str) {
	size_t end = str.find('=');
	if (end != string::npos) {
		key = str.substr(0, end);
		value = str.substr(end + 1);
		size_t start = 0;
		while (start < value.size()) {
			int startIncrease = 1;
			if ((end = value.find('+', start)) == string::npos) {
				startIncrease = 3;
				if ((end = value.find("%20", start)) == string::npos)
					end = value.size();
			}
			string tag = value.substr(start, end - start);
			if (!tag.empty())
				valueSegments.push_back(tag);
			start = end + startIncrease;
		}
		valueSegments.shrink_to_fit();
	}
	whole = std::move(str);
}
quasar::URI::querySegment::operator string() const { return whole; }
std::string quasar::URI::querySegment::operator()() const { return whole; }
std::string quasar::URI::querySegment::operator[](int i) const { return valueSegments[i]; }
quasar::URI::querySegment::operator string& () { return whole; }
std::string& quasar::URI::querySegment::operator()() { return whole; }
std::string& quasar::URI::querySegment::operator[](int i) { return valueSegments[i]; }

quasar::URI::operator string() const { return whole; }
std::string quasar::URI::operator()() const { return whole; }
quasar::URI::operator string& () { return whole; }
std::string& quasar::URI::operator()() { return whole; }

std::ostream& operator<<(std::ostream& os, const quasar::URI::querySegment& q) {
	os << q.whole;
	return os;
}
std::ostream& operator<<(std::ostream& os, const quasar::URI& u) {
	os << u.whole;
	return os;
}

std::string quasar::URI::percentageEncode(string str) {
	std::stringstream ss;
	for (auto c : str) {
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
			ss << c;
		else
			ss << '%' << std::hex << (int)c << std::dec;
	}
	return ss.str();
}
std::string quasar::URI::percentageDecode(string str) {
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

quasar::URI::URI() {}
quasar::URI::URI(string str) {
	size_t end = str.find(':');
	scheme = str.substr(0, end);
	size_t start = end + 1;
	if (str[start + 1] == '/') {
		start += 2;
		if ((end = str.find('/', start)) == string::npos)
			if ((end = str.find('?', start)) == string::npos) {
				if ((end = str.find('#', start)) == string::npos)
					end = str.size();
				else {
					authority = str.substr(start, end - start);
					fragment = str.substr(end + 1);
					goto skipPath;
				}
			}
		authority = str.substr(start, end - start);
		start = end + 1;
	}
	if (start < str.size()) {
		if ((end = str.find('?', start)) != string::npos) {
			path = str.substr(start, end - start);
			start = end + 1;
			if ((end = str.find('#', start)) != string::npos) {
				query = str.substr(start, end - start);
				fragment = str.substr(end + 1);
			}
			else
				query = str.substr(start);
		}
		else {
			if ((end = str.find('#', start)) != string::npos) {
				path = str.substr(start, end - start);
				fragment = str.substr(end + 1);
			}
			else
				path = str.substr(start);
		}
	}

skipPath:
	start = 0;
	if ((end = authority.find('@')) != string::npos) {
		userInfo = authority.substr(0, end);
		start = end + 1;
	}
	if ((end = authority.find_last_of(':')) != string::npos) {
		host = authority.substr(start, end - start);
		port = std::stoi(authority.substr(end + 1));
	}
	else
		host = authority.substr(start);

	start = 0;
	while (start < path.size()) {
		if ((end = path.find('/', start)) == string::npos)
			end = path.size();
		pathSegments.push_back(path.substr(start, end - start));
		start = end + 1;
	}
	pathSegments.shrink_to_fit();

	start = 0;
	while (start < query.size()) {
		if ((end = query.find('&', start)) == string::npos)
			end = query.size();
		querySegments.emplace_back(query.substr(start, end - start));
		start = end + 1;
	}
	querySegments.shrink_to_fit();
	whole = std::move(str);
}
#endif
#endif