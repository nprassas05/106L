#include <iostream>
#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <regex>
#include <iterator>
#include <string>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;


unordered_set<string> findWikiLinks(const string& page_html);
unordered_set<string> findWikiLinks2(const string& page_html);
string readFileIntoString(const string &file_name);

int main() {
    string s = readFileIntoString("quokka.txt");
    unordered_set<string> links = findWikiLinks2(s);
    for (auto link : links) {
        cout << link << endl;
    }
    return 0;
}

string readFileIntoString(const string &file_name) {
    std::ifstream input(file_name);
    if (!input) {
        throw std::runtime_error("could not open file " + file_name);
    }

    string line;
    string file_contents = "";

    while (getline(input, line)) {
        file_contents += line + " ";
    }

    return file_contents;
}

unordered_set<string> findWikiLinks(const string& page_html) {
    string link_start_str = "<a href=\"/wiki/";
    char close_quote_ch = '\"';
    unordered_set<string> ret;

    auto iter_start = page_html.begin();
    auto iter_end = page_html.end();

    while (iter_start != iter_end) {
        auto iter_link_start = std::search(iter_start, iter_end,
                                           link_start_str.begin(),
                                           link_start_str.end());
        if (iter_link_start == iter_end) {
            break;
        }

        auto iter_txt_start = iter_link_start + link_start_str.length();
        auto iter_txt_end = std::find(iter_txt_start, iter_end, close_quote_ch);

        if (iter_txt_end == iter_end) {
            break;
        }

        string link(iter_txt_start, iter_txt_end);
        if (!link.empty() && std::all_of(link.begin(), link.end(),
                    [](char c) {return c != ':' && c != '#';})) {
            ret.insert(link);
        }

        iter_start = iter_txt_end + 1;
   }

   return ret;
}

unordered_set<string> findWikiLinks2(const string& page_html) {
    unordered_set<string> ret;
    std::regex atag_reg("<a href=\"/wiki/([^\":#]+)\"[^>]*>[^<]+</a>");

    auto link_matches_begin = std::sregex_iterator(page_html.begin(), page_html.end(), atag_reg);
    auto link_matches_end = std::sregex_iterator();

    for (std::sregex_iterator iter = link_matches_begin; iter != link_matches_end; ++iter) {
          std::smatch match = *iter;
          ret.insert(match[1].str());
    }

    return ret;
}
