#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include "../../includes/sql/sql.h"
using namespace std;

const vectorstr command_list = {
"make table student fields 	fname, 			lname, 		major, 				age", 
"insert into student values 	Flo, 			Yao, 		CS, 				20", 
"insert into student values 	\"Flo\", 			\"Jackson\", 	Math,	 			21", 
"insert into student values 	Calvin, 		Woo, 		Physics,			22", 
"insert into student values 	\"Anna Grace\", 	\"Del Rio\", 	CS,	 				22", 
"select * from student"
};

bool test_batch1(bool debug=false){
  SQL sql("batchfile.txt");
}
bool test_batch2(bool debug=false){
  SQL sql("batchfile2.txt");
}
bool test_quote(bool debug=false){
  SQL sql;
  Table t;
  for (int i=0; i < command_list.size(); i++){
      cout<<">"<< command_list[i] << endl;
      cout<<sql.command(command_list[i]);
  }
  cout<<">"<< "select * from student where lname = \"Del Rio\"" << endl;
  cout<<sql.command("select * from student where lname = \"Del Rio\"");
  return true;
}

TEST(TEST_B, TestB) {
  EXPECT_EQ(1, test_quote(false));
  EXPECT_EQ(1, test_batch1(false));
  EXPECT_EQ(1, test_batch2(false));
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

