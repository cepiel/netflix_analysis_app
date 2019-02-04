/*main.cpp*/

//
// Netflix movie review analysis.
//
// << LUCAS CEPIEL >>
// U. of Illinois, Chicago
// CS 341: Fall 2018
// Project 02
//

#include <math.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <chrono>
#include <stdio.h>
#include <cstring>
#include <string>


using namespace std;



//////////////////////////////////////
class Review
{
private:
  int reviewID; 
  int movieID; 
  int rating; 
  int userID;
  string date;

public:
  
  
  Review (int review, int movie, int user, int rate, string year) : reviewID(review), movieID(movie), userID(user), rating(rate), date(year)
  { }

  
  int getMovieID(){
    return movieID;
  }
  
  int getRating(){
    return rating;
  }
  
  int getUserID(){
    return userID;
  }
  string getDate(){
    return date;
  }

};



//////////////////////////////////////
class Movie
{
private:
  int movieID; 
  string movieName; 
  int yearPublished;
  
  float totalStars;
  double counter;
  double average;
  int ones, twos, threes, fours, fives;
  
public:
  
  
  Movie (int id, string name, int year) : movieID(id), movieName(name), yearPublished(year)
  {
    //obligatory 'zero' initialization of variables
    counter=0.0;
    totalStars=0.0;
    average=0.0;
    ones=0;    twos=0;    threes=0;    fours=0;    fives=0;
    
  }

  void incrementCounter(){ //counter for total number of reviews
    counter++;
  }
  
  void addStars(double stars){ //add rating to totalStars and also increment invidiual star counts
    if(stars==1){
      ones++;
    }
    else if(stars==2){
      twos++;
    }
    else if(stars==3){
      threes++;
    }
    else if(stars==4){
      fours++;
    }
    else if(stars==5){
      fives++;
    }
    
    totalStars += stars;
  }
  
  double getAverage(){  //returns rating
    average = totalStars/counter;
    
    if(isnan(average)){
      return 0;
    }
    return average;
  }
  
  double getNumReviews(){  //returns num reviews
    return counter;
  }
  
  double getTotalStars(){  //returns totalStars
    return totalStars;
  }
  
  string getName(){
    return movieName;
  }
  
  int getMovieID(){
    return movieID;
  }
  
  string getMovieName(){
    return movieName;
  }
  
  int getYear(){
    return yearPublished;
  }
  
  int get1Star(){
    return ones;
  }
  int get2Star(){
    return twos;
  }
  int get3Star(){
    return threes;
  }
  int get4Star(){
    return fours;
  }
  int get5Star(){
    return fives;
  }


};



//////////////////////////////////////////
int main()
{
  cout << "** Netflix Movie Review Analysis **" << endl;
  cout << endl;

  //
  // input the filenames to process:
  //
  string moviesFN, reviewsFN;

  cin >> moviesFN;
  cin >> reviewsFN;

  cout << moviesFN << endl;
  cout << reviewsFN << endl;
  cout << endl;


  //start by reading .csv for MOVIES
  ifstream file(moviesFN);
  
  //relevant variables for scanning

  map<int, Movie> movies;
  map<int, Review> reviews;
  
  string movieID, movieName, yearPublished;
  string reviewID, rating, date, user;
  string line;
  
  
  if(!file.good()){
    cout <<"Cannot open file 1!" << endl;
    return -1;
  }
  
  getline(file, line);      //skips header
  while(getline(file,line))
  {
    stringstream ss(line);
    
    getline(ss, movieID, ',');
    getline(ss, movieName, ',');
    getline(ss, yearPublished, ',');

    movies.emplace(stoi(movieID), Movie(stoi(movieID), movieName, stoi(yearPublished)) );
    
    
  }//end scanning movies
  
  
 

  //read .csv for REVIEWS
  
  ifstream file2(reviewsFN);

  if(!file2.good()){
    cout <<"Cannot open file 2!" << endl;
    return -1;
  }
  
  getline(file2, line);      //skips header
  while(getline(file2,line))
  {
    stringstream ss(line);
    
    getline(ss, reviewID, ',');
    getline(ss, movieID, ',');
    getline(ss, user, ',');
    getline(ss, rating, ',');
    getline(ss, date, ',');

    reviews.emplace(stoi(reviewID),  Review(stoi(reviewID), stoi(movieID), stoi(user), stoi(rating), date) );
    
    
  }//end scanning reviews



  
  //cycle through reviews and add their stars to the movies

  for(auto const& r : reviews)
  {
    auto value = r.second;
    int ID = value.getMovieID();
    
    auto iter = movies.find(ID); //searches movies for review's movie
    
    if(iter != movies.end()){   //movieID found, increment counter and add the rating
      int rating = value.getRating();

      iter->second.incrementCounter(); //increment number of reviews
      iter->second.addStars(rating); //add stars to totalStars and individual star counts. average will be totalstars/counter
    }
    else
      cout << "Not found!" << endl;
  }
  
  
  
  
  //make a multimap of all movies, which organizes by average! insert old map

  multimap<double, Movie> rankedMovies;
  
  for(auto& kvp : movies){
    Movie m = kvp.second;
    double avg = m.getAverage();

    rankedMovies.insert(pair<double, Movie>(avg, m) );
  }
 
  
  
  //get top ten movies by cycling through ranked multimap of movies, backwards. Add top 10 movies to a vector (ignoring movies with 0 reviews)
  vector<Movie> topTen;
  
  int x=1;
  for (auto iter = rankedMovies.rbegin(); iter != rankedMovies.rend(); ++iter){
    Movie &m = iter->second;
    
    if(m.getNumReviews() != 0){ //ignore movies with 0 reviews, i.e. NAN average movies that appeared at end
      topTen.push_back(m);
      x++;
    }
     if(x==11){
       break;
     }
  }
  
  
  //Output TOP 10 movies by average reviews!

  cout << ">> Top-10 Movies <<\n" << endl;
  cout << "Rank\tID\tReviews\tAvg\tName" << endl;
  
  int y=1;
  for(Movie& m : topTen){ 
    
      cout << y << ".\t"
        << m.getMovieID() << "\t"
        << m.getNumReviews() << "\t"
        << m.getAverage() << "\t'"
        << m.getMovieName() << "'"
        << endl;
      y++;
  }
     
      
  
  
  //loop to ask for movie IDs to output movie stats (0 to stop)
  //Movie IDs are < 100,000, and review IDs are >= 100,000.  Figure 1 (the first screenshot in the handout) is incorrect.

  
  cout << "\n>> Movie and Review Information <<" << endl;
  int search=1;  
    
  while(1){
    
    cout << "\nPlease enter a movie ID (<100,000), a review ID (>= 100,000), 0 to stop> ";
    cin >> search;
    
    if(search < 100000 && search >0){  //searching for a movie
      
    auto iter = movies.find(search); //searches movies for review's movie
    
    if(iter != movies.end()){   //movie found
      Movie& m = iter->second;
      
      cout << "\nMovie:\t" << "'" << m.getMovieName() << "'"
           << "\nYear:\t" << m.getYear()
           << "\nAvg rating:\t" << m.getAverage()
           << "\nNum reviews:\t" << m.getNumReviews()
           << "\n 1 star:\t" << m.get1Star()
           << "\n 2 stars:\t" << m.get2Star()
           << "\n 3 stars:\t" << m.get3Star()
           << "\n 4 stars:\t" << m.get4Star()
           << "\n 5 stars:\t" << m.get5Star() << endl;

    }
    else
      cout << "movie not found..." << endl;
    }
    else if(search >= 100000){ //searching for a review
      
    auto iter = reviews.find(search); //searches movies for review's movie
    
    if(iter != reviews.end()){   //movieID found, increment counter and add the rating
      Review& r = iter->second;
      
      auto iter = movies.find(r.getMovieID()); //look up movie name for printing out

      cout << "\nMovie:\t" << r.getMovieID() << " (" <<  iter->second.getMovieName() << ")"
           << "\nNum stars:\t" << r.getRating()
           << "\nUser id:\t" << r.getUserID()
           << "\nDate:\t" << r.getDate() << endl;

    }
    else
      cout << "review not found..." << endl;
    }
    else if(search == 0){   //exit the program
      break;
    }
    else if(search < 0){  //invalid
      cout << "\n**invalid id..." << endl;
    }
    else  //necessary last else
      cout << "\n**unknown input" << endl;

    
  }//end while
  
  
  
  
  //
  // done!!!!!
  //
  cout << "\n** DONE! **" << endl << endl;

  return 0;
}
