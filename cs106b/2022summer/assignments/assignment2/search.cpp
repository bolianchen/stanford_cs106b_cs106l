// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <cctype>
#include "error.h"
#include "filelib.h"
#include "simpio.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// Convert a input string to a cleaned version
// @s Input string
// Trims all punctuations from the end and start of s.
// Confirms the trimmed s contains at least one letter.
// Return empty string if there are no letters.
// Otherwise, convert the trimmed s to lower-case
string cleanToken(string s)
{
    string cleanedToken;
    int indexFirstLetter = -1, indexLastLetter = -1;
    bool foundFirst = false, foundLast = false;
    for (int i = 0; i < s.length(); i++) {
        if (!foundFirst && !ispunct(s[i])) {
            indexFirstLetter = i;
            foundFirst = true;
        }
        if (!foundLast && !ispunct(s[s.length() - 1 - i])) {
            indexLastLetter = s.length() - 1 - i;
            foundLast = true;
        }
    }
    if (!foundFirst) {
        return string();
    }
    cleanedToken = s.substr(indexFirstLetter, indexLastLetter - indexFirstLetter + 1);
    for (char c: cleanedToken) {
        if (isalpha(c)) {
            return toLowerCase(cleanedToken);
        }
    }
    return string();
}

// Splits text into tokens using space and clean them
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    string cleanedToken;
    for (string s: stringSplit(text, " ")) {
        cleanedToken = cleanToken(s);
        if (!cleanedToken.empty()) {
            tokens.add(cleanedToken);
        }
    }
    return tokens;
}

/* Build inverted index from a database file
 * @dbfile The database file whose lines are grouped into pairs.
 * 		   The first line of the pair is a page URL; the second line is the body text.
 * @index The inverted index to build. It map a keyword to all the URLs where the keyword occurs.
 */

int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;
    if (!openFile(in, dbfile)) {
        error("Cannot open file named " + dbfile);
    }
    Vector<string> lines;
    readEntireFile(in, lines);
    int numLines = lines.size();

    string website;
    string websiteText;
    Set<string> matchedWebsites;

    for (int i = 0; i < numLines; i += 2) {
        website = lines.get(i);
        websiteText = lines.get(i + 1);
        for (string token: gatherTokens(websiteText)) {
            matchedWebsites = index.get(token);
            matchedWebsites.add(website);
            index.put(token, matchedWebsites);
        }
    }

    return numLines / 2;
}

/* Return the matches of a single or a sequence of search terms.
 * @index The built inverted index
 * @query A single search term or a sequence of search terms, each of which may or may not be preceded by a modifier
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    string cleanedTerm;
    for (string searchTerm: stringSplit(query, " ")) {
        cleanedTerm = cleanToken(searchTerm);
        if (cleanedTerm.length() != 0) {
            if (searchTerm[0] == '+') {
                result.intersect(index.get(cleanToken(searchTerm)));
            } else if (searchTerm[0] == '-') {
                result.difference(index.get(cleanToken(searchTerm)));
            } else {
                result.unionWith(index.get(cleanToken(searchTerm)));
            }
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    Map<string, Set<string>> index;
    cout << "Stand by while building index..." << endl;
    int numPages = buildIndex(dbfile, index);
    cout << "Indexed " << numPages << " pages containing " << index.size() << " unique terms." << endl;

    string query;
    Set<string> matches;
    while (true) {
        query = getLine("Enter your search query (RETURN to quit)");
        if (query.length() == 0) {
            break;
        }
        matches = findQueryMatches(index, query);
        cout << "Found " << matches.size() << " matching pages" << endl
             << matches << endl << endl;
    }
    cout << "All done!" << endl;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

// TODO: add your test cases here
STUDENT_TEST("cleanToken on strings with punctuation at either beginning or end") {
    EXPECT_EQUAL(cleanToken("*&^hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD%$$"), "world");
}
STUDENT_TEST("cleanToken on strings only one character") {
    EXPECT_EQUAL(cleanToken("O"), "o");
    EXPECT_EQUAL(cleanToken("c"), "c");
    EXPECT_EQUAL(cleanToken("$"), "");
    EXPECT_EQUAL(cleanToken("1"), "");
}

STUDENT_TEST("gatherTokens on a simple sentence") {
    Set<string> expected = {"i", "am", "superman"};
    EXPECT_EQUAL(gatherTokens("I am Superman"), expected);
}

STUDENT_TEST("gatherTokens from all punctuations") {
    Set<string> expected = {};
    EXPECT_EQUAL(gatherTokens("^&* $*$ @##"), expected);
}

STUDENT_TEST("gatherTokens from partial punctuations, letters and numbers") {
    Set<string> expected = {"a", "a$1"};
    EXPECT_EQUAL(gatherTokens("^&* $A$ @1# A$1"), expected);
}

STUDENT_TEST("buildIndex from tiny.txt, websites containing fish") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expectedWebsites = {"www.shoppinglist.com", "www.dr.seuss.net", "www.bigbadwolf.com"};
    EXPECT_EQUAL(index.get("fish"), expectedWebsites);
}

STUDENT_TEST("buildIndex from tiny.txt, websites containing bread") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expectedWebsites = {"www.shoppinglist.com"};
    EXPECT_EQUAL(index.get("bread"), expectedWebsites);
}

STUDENT_TEST("buildIndex from tiny.txt, websites containing red") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expectedWebsites = {"www.rainbow.org", "www.dr.seuss.net"};
    EXPECT_EQUAL(index.get("red"), expectedWebsites);
}

STUDENT_TEST("buildIndex from tiny.txt, websites containing red") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expectedWebsites = {"www.rainbow.org", "www.dr.seuss.net"};
    EXPECT_EQUAL(index.get("red"), expectedWebsites);
}

STUDENT_TEST("buildIndex from tiny.txt, returned websites when using empty string as the query keyword") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> expectedWebsites = {};
    EXPECT_EQUAL(index.get(""), expectedWebsites);
}

STUDENT_TEST("Time trials of building inverted index for res/tiny.txt") {
    Map<string, Set<string>> index;
    TIME_OPERATION("tiny", buildIndex("res/tiny.txt", index));
}

STUDENT_TEST("Time trials of building inverted index for res/website.txt") {
    Map<string, Set<string>> index;
    TIME_OPERATION("tiny", buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches from tiny.txt, invalid queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesWithEmptyQuery = findQueryMatches(index, "");
    EXPECT_EQUAL(matchesWithEmptyQuery.size(), 0);
    Set<string> matchesWithInvalid1 = findQueryMatches(index, "&$1 +123 -%#");
    EXPECT_EQUAL(matchesWithInvalid1.size(), 0);
    Set<string> matchesWithInvalid2 = findQueryMatches(index, "+=-&$1 +123 +%#");
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries interleaved with invalid terms") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red +&34 fish -**88");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "+-*)( red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "--+*&^ red -fish +-+134");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

STUDENT_TEST("findQueryMatches from tiny.txt, matches of compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matches1 = findQueryMatches(index, "fish -+bread I**");
    Set<string> expectedMatches1 = {"www.dr.seuss.net", "www.bigbadwolf.com"};
    EXPECT_EQUAL(matches1, expectedMatches1);
    Set<string> matches2 = findQueryMatches(index, "red -blue +green");
    Set<string> expectedMatches2 = {};
    EXPECT_EQUAL(matches2, expectedMatches2);
    Set<string> matches3 = findQueryMatches(index, "red -blue green");
    Set<string> expectedMatches3 = {"www.rainbow.org"};
    EXPECT_EQUAL(matches3, expectedMatches3);
}
