#define QUASAR_IMPLEMENTATION
#include "URI.h"
#include <iostream>
int main(int argc, char* argv[]) {
	using std::cout;
	quasar::URI url("https://johnBlandguy:isurehope@chanty.cultists.com:666/test/path/of/doom?prev=4.01747446788347%2B31354386+ratio&tags=sacrificial_iguana%20order%3Apopular&page=12&yellingfroggo#fragmentTest");
	cout << "whole: " << url << '\n';
	cout << "scheme: " << url.scheme << '\n';
	cout << "authority: " << url.authority << '\n';
	cout << "\tuserInfo: " << url.userInfo << '\n';
	cout << "\thost: " << url.host << '\n';
	cout << "\tport: " << url.port << '\n';
	cout << "path: " << url.path << '\n';
	cout << "pathSegments: " << '\n';
	for (auto& s : url.pathSegments)
		cout << '\t' << s << '\n';
	cout << "query: " << url.query << '\n';
	cout << "querySegments: " << '\n';
	for (auto& s : url.querySegments) {
		cout << '\t' << s << '\n';
		if (!s.key.empty()) {
			cout << "\t\t" << s.key << '\n';
			cout << "\t\t" << s.value << '\n';
			for (auto& v : s.valueSegments)
				cout << "\t\t\t" << v << '\n';
		}
	}
	cout << "fragment: " << url.fragment << '\n';
}