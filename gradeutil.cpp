 /*gradeutil.cpp*/

//
// Imaad Sohraab

// GradeUtil API
// 

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "gradeutil.h"

using namespace std;

void Dept::addCourse(Course course)
{
	Courses.push_back(course);
}

void College::addDepartment(Dept department)
{
	//College.push_back(department);
	Depts.push_back(department);
}



//
// API:
//

// 
// ParseCourse:
//
// Parses a CSV (comma-separated values) line into a Course
// object, which is then returned.  The given line must have
// the following format:
//
//   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor
//
// Example:
//   BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington
//
// Note the lack of spaces, except perhaps in the title.
// If the given line does not have this format, the behavior 
// of the function is undefined (it may crash, it may throw
// an exception, it may return).
//
Course ParseCourse(string csvline)
{
     //string s = "BIOE,101,01,Intro to Bioengineering,22,8,2,1,0,1,0,0,0,5,Eddington"; 
    istringstream ss(csvline);
    string token;
    string array[15];
    int i = 0;
    
    while(getline(ss, token, ',')) 
    {
  //      cout << token << '\n';
        array[i] = token;
        i++;
    }
    
    Course temp;
    
    //   Dept,Number,Section,Title,A,B,C,D,F,I,NR,S,U,W,Instructor

    
    temp.Dept = array[0];
    temp.Number = stoi(array[1]);
    temp.Section = stoi(array[2]);
    temp.Title =  array[3];
    temp.NumA = stoi(array[4]);
    temp.NumB =  stoi(array[5]);
    temp.NumC =  stoi(array[6]);
    temp.NumD =  stoi(array[7]);
    temp.NumF =  stoi(array[8]);
    temp.NumI =  stoi(array[9]);
    temp.NumNR =  stoi(array[10]);
    temp.NumS =  stoi(array[11]);
    temp.NumU =  stoi(array[12]);
    temp.NumW =  stoi(array[13]);
    temp.Instructor = array[14];
   
  
  return temp;
}


//
// GetDFWRate:
//
// Returns the DFW rate as a percentage for a given course, 
// department, or college.  For a course whose grading type 
// is defined as Course::Letter, the DFW rate is defined as
// 
//   # of D grades + F grades + Withdrawals
//   -------------------------------------- * 100.0
//   # of A, B, C, D, F grades + Withdrawls
//
// The numerator is returned via the reference parameter DFW;
// the denominator is returned via the reference parameter N.
// If the course grading type is not Course::Letter, the DFW
// rate is 0.0, and parameters DFW and N are set to 0.  
//
// When computed for a dept or college, all courses of type 
// Course::Letter are considered in computing an overall DFW 
// rate for the dept or college.  The reference parameters 
// DFW and N are also computed across the dept or college.
//
double GetDFWRate(const Course& c, int& DFW, int& N)
{
  DFW = 0;
  N   = 0;
  

  Course::GradingType gradeType = c.getGradingType();
     
  if (gradeType != Course::Letter)
    return 0.0;
	
	
  DFW = (c.NumD + c.NumF + c.NumW);
  N = (c.NumA + c.NumB + c.NumC + c.NumD + c.NumF + c.NumW);
    
    if (N == 0) // just in case, guard against divide by 0:
    return 0.0;
     
  return (static_cast<double>(DFW) / static_cast<double>(N)) * 100.0;

}

double GetDFWRate(const Dept& dept, int& DFW, int& N)
{
   DFW = 0;
   N   = 0;
  
   for(const Course& ex : dept.Courses)
   {
       if(ex.getGradingType() != Course::Letter)
          continue;
       
      DFW = DFW + (ex.NumD + ex.NumF + ex.NumW);
      N = N + (ex.NumA + ex.NumB + ex.NumC + ex.NumD + ex.NumF + ex.NumW);
   }
    
    if(N == 0)
        return 0.0;
    
  
  return (static_cast<double>(DFW) / static_cast<double>(N)) * 100.0;

}

double GetDFWRate(const College& college, int& DFW, int& N)
{
    DFW = 0;
      N   = 0;
	for(const Dept& ex : college.Depts)
	{
		for(const Course& ex2: ex.Courses)
		{
			if(ex2.getGradingType() != Course::Letter)
				continue;
		  DFW = DFW + (ex2.NumD + ex2.NumF + ex2.NumW);
		  N = N + (ex2.NumA + ex2.NumB + ex2.NumC + ex2.NumD + ex2.NumF + ex2.NumW);

		}
	}
	
	if(N != 0)
	{
		  return (static_cast<double>(DFW) / static_cast<double>(N)) * 100.0;
	}
	else
        return 0.0;
}
    
 


//
// GetGradeDistribution
//
// Returns an object containing the grade distribution for a given
// course, dept or college.  For a course whose grading type is
// defined as Course::Letter, the grade distribution is defined by
// the following values:
//
//   N: the # of A, B, C, D, F grades
//   NumA, NumB, NumC, NumD, NumF: # of A, B, C, D, F grades
//   PercentA, PercentB, PercentC, PercentD, PercentF: % of A, B, 
//     C, D, F grades.  Example: PercentA = NumA / N * 100.0
//
// If the course grading type is not Course::Letter, all values
// are 0.  When computed for a dept or college, all courses of
// type Course::Letter are considered in computing an overall 
// grade distribution for the dept or college.
//
GradeStats GetGradeDistribution(const Course& c)
{
  
	Course::GradingType gradeType = c.getGradingType();
	GradeStats gStats;

   if(gradeType != Course::Letter)
          return gStats;
	//GradeStats gStats;
	gStats.N = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
	
	gStats.NumA = c.NumA;
	gStats.NumB = c.NumB;
	gStats.NumC = c.NumC;
	gStats.NumD = c.NumD;
	gStats.NumF = c.NumF;
	
	gStats.PercentA = (static_cast<double>(gStats.NumA) / static_cast<double>(gStats.N)) * 100.0;
	gStats.PercentB = (static_cast<double>(gStats.NumB) / static_cast<double>(gStats.N)) * 100.0;
	gStats.PercentC = (static_cast<double>(gStats.NumC) / static_cast<double>(gStats.N)) * 100.0;
	gStats.PercentD = (static_cast<double>(gStats.NumD) / static_cast<double>(gStats.N)) * 100.0;
	gStats.PercentF = (static_cast<double>(gStats.NumF) / static_cast<double>(gStats.N)) * 100.0;

  
  return gStats;  
}

GradeStats GetGradeDistribution(const Dept& dept)
{
  Course cTotal;
	for(const Course& temp_course : dept.Courses)
	{
		if(temp_course.getGradingType() != Course::Letter)
			continue;
		
		cTotal.NumA = cTotal.NumA + temp_course.NumA;
		cTotal.NumB = cTotal.NumB + temp_course.NumB;
		cTotal.NumC = cTotal.NumC + temp_course.NumC;
		cTotal.NumD = cTotal.NumD + temp_course.NumD;
		cTotal.NumF = cTotal.NumF + temp_course.NumF;
	}
 
  
  return GetGradeDistribution(cTotal);
}

GradeStats GetGradeDistribution(const College& college)
{
  

  Course cTotal;
	
  for(const Dept& ex : college.Depts)
  {
	  for(const Course& temp_course : ex.Courses)
		  {
		  if(temp_course.getGradingType() != Course::Letter)
				continue;
		
		  cTotal.NumA = cTotal.NumA + temp_course.NumA;
		  cTotal.NumB = cTotal.NumB + temp_course.NumB;
		  cTotal.NumC = cTotal.NumC + temp_course.NumC;
		  cTotal.NumD = cTotal.NumD + temp_course.NumD;
		  cTotal.NumF = cTotal.NumF + temp_course.NumF;
		  }
	
  }
	
  
  return GetGradeDistribution(cTotal);
}


//
// FindCourses(dept, courseNumber)
//
// Searches the courses in the department for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, and returned in
// ascending order by section number.
//
vector<Course> FindCourses(const Dept& dept, int courseNumber)
{
  vector<Course>  courses;
	
  for(const Course& temp_course: dept.Courses)
  {
	  if(temp_course.Number == courseNumber)
	  {
		  courses.push_back(temp_course);
	  }
  }
	
	// organize by ascending order by Section
 
	sort(courses.begin(), courses.end(), 
      [](const Course& course1, const Course& course2)
      {
        
          if (course1.Section < course2.Section)
            return true;
          else
            return false;
      }
    );
	
	
	//  
    
  return courses;
}


//
// FindCourses(dept, instructorPrefix)
//
// Searches the courses in the department for those whose 
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".
//
// If none are found, then the returned vector is empty.  If 
// one or more courses are found, copies of the course objects
// are returned in a vector, with the courses appearing in 
// ascending order by course number.  If two courses have the 
// same course number, they are given in ascending order by 
// section number.  Note that courses are NOT sorted by instructor
// name.
//
vector<Course> FindCourses(const Dept& dept, string instructorPrefix)
{
  vector<Course>  courses;
  
  int lengthOfInstructor = instructorPrefix.length();
	
  for(const Course& temp_course : dept.Courses)
  {
	  // Goes from 0 to length of the string
	  if(temp_course.Instructor.substr(0, lengthOfInstructor) == instructorPrefix)
	  {
		  courses.push_back(temp_course);
	  }
  }
	
	sort(courses.begin(), courses.end(), 
      [](const Course& course1, const Course& course2)
      {
		  if(course1.Number < course2.Number)
			  return true;
          
          else if(course1.Number > course2.Number)
            return false;
		  else 
		  {
			  if(course1.Section < course2.Section)
				  return true;
			  else
				  return false;
		  }
      }
    );

  return courses;
}


//
// FindCourses(college, courseNumber)
//
// Searches for all courses in the college for those that match
// the given course number.  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department, then course number,
// and then section number.
//
vector<Course> FindCourses(const College& college, int courseNumber)
{
  vector<Course>  courses;
  
  // For every college, search courses that match the # and put them in 1 vector
  
	for(const Dept& temp_dept: college.Depts)
	{
		vector<Course> perDept = FindCourses(temp_dept, courseNumber);
		
		for(const Course& temp_course: perDept)
		{
			courses.push_back(temp_course);
		}
	}
   sort(courses.begin(), courses.end(), 
      [](const Course& course1, const Course& course2)
      {
        if (course1.Dept < course2.Dept)
			return true;
		else if (course1.Dept > course2.Dept)
			return false;
		else
		{
			if(course1.Section < course2.Section)
				return true;
			else 
				return false;
		}
	  }
	);
  return courses;
}


//
// FindCourses(college, instructorPrefix)
//
// Searches all the courses in the college for those whose
// instructor name starts with the given instructor prefix.
// For example, the prefix "Re" would match instructors "Reed"
// and "Reynolds".  If none are found, then the returned
// vector is empty.  If one or more courses are found, copies of
// the course objects are returned in a vector, with the courses
// appearing in ascending order by department,
// then course number, and then section.
//
vector<Course> FindCourses(const College& college, string instructorPrefix)
{
  vector<Course>  courses;
  
  // For every college, search courses that match the # and put them in 1 vector
  
	for(const Dept& temp_dept: college.Depts)
	{
		vector<Course> perDept = FindCourses(temp_dept, instructorPrefix);
		
		for(const Course& temp_course: perDept)
		{
			courses.push_back(temp_course);
		}
	}
	
	sort(courses.begin(), courses.end(), 
      [](const Course& course1, const Course& course2)
      {
	
		  if(course1.Dept < course2.Dept)
			  return true;
		  else if(course1.Dept > course2.Dept)
			  return false;
		  else
		 
				{
					if (course1.Number < course2.Number)
						return true;
					else if (course1.Number > course2.Number)
						return false;
						 else
						 {
							if(course1.Section < course2.Section)
								return true;
							else 
								return false;
			}
		}
			  
       
	  }
	);
    
  return courses;
}