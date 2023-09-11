#include "URI.h"
using std::string;
using namespace quasar;
URI::querySegment::querySegment(string str) {
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
			valueSegments.push_back(value.substr(start, end - start));
			start = end + startIncrease;
		}
		valueSegments.shrink_to_fit();
	}
	whole = std::move(str);
}
URI::querySegment::operator string() const { return whole; }
string URI::querySegment::operator()() const { return whole; }
string URI::querySegment::operator[](int i) const { return valueSegments[i]; }
URI::querySegment::operator string& () { return whole; }
string& URI::querySegment::operator()() { return whole; }
string& URI::querySegment::operator[](int i) { return valueSegments[i]; }

URI::operator string() const { return whole; }
string URI::operator()() const { return whole; }
URI::operator string& () { return whole; }
string& URI::operator()() { return whole; }

std::ostream& operator<<(std::ostream& os, const quasar::URI::querySegment& q) {
	os << q.whole;
	return os;
}
std::ostream& operator<<(std::ostream& os, const quasar::URI& u) {
	os << u.whole;
	return os;
}

string URI::percentageEncode(string str) {
	std::stringstream ss;
	for (auto c : str) {
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
			ss << c;
		else
			ss << '%' << std::hex << (int)c;
	}
	return ss.str();
}
string URI::percentageDecode(string str) {
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

URI::URI() {}
URI::URI(string str) {
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