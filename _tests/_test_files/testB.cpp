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
"select * from student",
"select * from student where lname = \"Del Rio\""
};

bool test_stub(bool debug=false){
  // SQL sql("batchfile.txt");
  SQL sql;
  Table t;
  for (int i=0; i < command_list.size(); i++){
      cout<<">"<< command_list[i] << endl;
      cout<<sql.command(command_list[i]);
  }
  return true;
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(false));
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

