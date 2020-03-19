#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <ostream>
#include <unordered_map>
#include "wikiscraper.h"


using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;


/*
 * This function returns the number of elements in common between
 * the two given unordered sets.
 */
template <typename ValueType>
int numInCommon(const unordered_set<ValueType> set1,
                const unordered_set<ValueType> set2) {
    int numCommon = 0;
    for (const auto &elem: set1) {
        if (set2.count(elem)) {
            numCommon += 1;
        }
    }

    return numCommon;
}

/*
 * This function compares two partial wiki ladders and returns whether the
 * first ladder has less links in common with the second ladder.
 *
 *
 * Pre-conditions:
 * ladder1.size() > 0
 * ladder2.size() > 0
 *
 */
bool compareLadders(const vector<string>& ladder1, const vector<string>& ladder2,
                    const unordered_set<string> linksEndPage, WikiScraper &scraper) {
    string page1 = *(ladder1.end() - 1);
    string page2 = *(ladder2.end() - 1);

    unordered_set<string> links1 = scraper.getLinkSet(page1);
    unordered_set<string> links2 = scraper.getLinkSet(page2);

    int numCommonLinks1 = numInCommon<string>(links1, linksEndPage);
    int numCommonLinks2 = numInCommon<string>(links2, linksEndPage);

    return numCommonLinks1 < numCommonLinks2;
}

/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    WikiScraper scraper;
    unordered_set<string> linksEndPage = scraper.getLinkSet(end_page);

    auto cmpFunc = [&linksEndPage, &scraper](vector<string> &l1, vector<string> &l2) -> bool {
        return compareLadders(l1, l2, linksEndPage, scraper);
    };

    priority_queue<vector<string>, vector<vector<string>>,
            decltype (cmpFunc)> ladderQueue(cmpFunc);

    vector<string> startLadder = {start_page};
    unordered_set<string> visitedPages = {start_page};

    ladderQueue.push(startLadder);

    while (!ladderQueue.empty()) {
        vector<string> currLadder = ladderQueue.top();
        ladderQueue.pop();

        unordered_set<string> linksCurrLadder = scraper.getLinkSet(*(currLadder.end() - 1));
        if (linksCurrLadder.count(end_page)) {
            currLadder.push_back(end_page);
            return currLadder;
        } else {
            for (const auto& link : linksCurrLadder) {
                if (!visitedPages.count(link)) {
                    vector<string> ladderCopy(currLadder);
                    ladderCopy.push_back(link);
                    ladderQueue.push(ladderCopy);
                    visitedPages.insert(link);
                }
            }
        }
    }

    return {};
}


std::ostream& operator<<(std::ostream& stream, vector<string> &vec) {
    stream << "{";
    if (!vec.empty()) {
        for (auto iter = vec.begin(); iter != vec.end() - 1; ++iter) {
            stream << *iter << ", ";
        }
        stream << *(vec.end() - 1);
    }
    stream << "}";
    return stream;
}


int main() {
    auto ladder = findWikiLadder("Albert_Einstein", "Scientology");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";

        // Print the ladder here!
        cout << ladder << endl;
    }

    return 0;
}

