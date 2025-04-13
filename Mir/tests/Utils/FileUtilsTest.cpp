#include <gtest/gtest.h>
#include "Mir/Utils/Utils.h"
#include "Mir/Log.h"
#include <filesystem>
#include <fstream>
#include <sstream>

class FileUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize log if needed
        Mir::Log::Init();
    }
};

// Test reading lines from an empty stream
TEST_F(FileUtilsTest, ReadLinesFromEmptyStream) {
    std::istringstream emptyStream("");
    auto lines = Mir::Utils::File::readLines(emptyStream);
    EXPECT_TRUE(lines.empty());
}

// Test reading lines with various line ending formats
TEST_F(FileUtilsTest, ReadLinesWithDifferentLineEndings) {
    std::istringstream stream("Line 1\r\nLine 2\nLine 3\r\n");
    auto lines = Mir::Utils::File::readLines(stream);
    
    ASSERT_EQ(3, lines.size());
    EXPECT_EQ("Line 1", lines[0]);
    EXPECT_EQ("Line 2", lines[1]);
    EXPECT_EQ("Line 3", lines[2]);
}

// Test reading only whitespace lines
TEST_F(FileUtilsTest, ReadLinesWithWhitespace) {
    std::istringstream stream("\n  \n\t\n");
    auto lines = Mir::Utils::File::readLines(stream);
    
    ASSERT_EQ(3, lines.size());
    EXPECT_EQ("", lines[0]);
    EXPECT_EQ("  ", lines[1]);
    EXPECT_EQ("\t", lines[2]);
}

// Test reading an empty file as a string
TEST_F(FileUtilsTest, ReadEmptyFileFromStream) {
    std::istringstream emptyStream("");
    std::string content = Mir::Utils::File::readFile(emptyStream);
    
    EXPECT_TRUE(content.empty());
}

// Test reading a very large stream
TEST_F(FileUtilsTest, ReadLargeStream) {
    std::string largeString(100000, 'X');
    std::istringstream largeStream(largeString);
    
    std::string content = Mir::Utils::File::readFile(largeStream);
    
    EXPECT_EQ(largeString.size(), content.size());
    EXPECT_EQ(largeString, content);
}

// Test reading from a bad stream
TEST_F(FileUtilsTest, ReadFromBadStream) {
    std::istringstream stream("Test data");
    
    // Make the stream bad
    stream.setstate(std::ios::badbit);
    
    // Try to read from the bad stream
    auto lines = Mir::Utils::File::readLines(stream);
    EXPECT_TRUE(lines.empty());
    
    // Reset stream and try readFile
    stream.clear();
    stream.str("Test data");
    stream.setstate(std::ios::badbit);
    
    std::string content = Mir::Utils::File::readFile(stream);
    EXPECT_TRUE(content.empty());
}

TEST_F(FileUtilsTest, SplitLine) {
    std::string testString = "\"data1,data2\",data3,data4";
    std::string testString2 = "data1,data2,data3,data4";
    std::vector<std::string> tokens = Mir::Utils::Text::splitLine(testString, ',');
    EXPECT_EQ(3,tokens.size());
    
    tokens = Mir::Utils::Text::splitLine(testString2, ',');
    EXPECT_EQ(4, tokens.size());

    tokens = Mir::Utils::Text::splitLine(testString2);
    EXPECT_EQ(1, tokens.size());

}

TEST_F(FileUtilsTest, FileExtensions){
    std::filesystem::path filePath1 = "test.txt";
    std::filesystem::path filePath2 = "noextension";
    std::filesystem::path filePath3 = "path/to/file.typ";
    std::filesystem::path filePath4 = ".gitignore";
    std::filesystem::path filePath5 = "testdata.iom";

    EXPECT_EQ(".txt", Mir::Utils::File::getFileExtension(filePath1));
    EXPECT_EQ("", Mir::Utils::File::getFileExtension(filePath2));
    EXPECT_EQ(".typ", Mir::Utils::File::getFileExtension(filePath3));
    EXPECT_EQ("", Mir::Utils::File::getFileExtension(filePath4)); 
    EXPECT_EQ(".iom", Mir::Utils::File::getFileExtension(filePath5));

    EXPECT_TRUE(Mir::Utils::File::hasExtension(filePath1, ".txt"));
    EXPECT_TRUE(Mir::Utils::File::hasExtension(filePath1, ".TXT")); 
    EXPECT_FALSE(Mir::Utils::File::hasExtension(filePath1, ".cpp"));
    EXPECT_FALSE(Mir::Utils::File::hasExtension(filePath2, ".txt"));
    EXPECT_TRUE(Mir::Utils::File::hasExtension(filePath3, ".typ"));
    EXPECT_FALSE(Mir::Utils::File::hasExtension(filePath4, ".gitignore"));
    EXPECT_TRUE(Mir::Utils::File::hasExtension(filePath5, ".iom"));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}