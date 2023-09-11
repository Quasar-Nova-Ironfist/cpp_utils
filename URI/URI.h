#ifndef ___QUASAR_URI_H___
#define ___QUASAR_URI_H___
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
#endif