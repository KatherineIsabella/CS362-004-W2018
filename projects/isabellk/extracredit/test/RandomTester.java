import java.io.IOException;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Random;

import org.junit.Test;

import junit.framework.TestCase;

public class RandomTester extends TestCase {

	public int generatePort(Random rand) {
		
		//According to https://www.ibm.com/support/knowledgecenter/SSGMCP_5.1.0/com.ibm.cics.ts.internet.doc/topics/dfhtl2d.html#dfhtl2d
		//port numbers can go up to 65535 so we want to test all from  0-65535
		int  portNum = rand.nextInt(65536);

		return portNum;
	}
	
	
	public String generateScheme(Random rand) {
		
		//randomly pick how long the scheme can be 0-9 chars
		int numberOfChars = rand.nextInt(10);
		String scheme = "";
		int i = 0;
		
		//go through and add each char to the scheme
		while (i < numberOfChars) {
			
			//get a number between 1-3
			int addOnChoice = rand.nextInt(3) + 1;
			
			//if we get a 1 then make it either a symbol or number to get added (33%)
			if(addOnChoice == 1) {
				
				if(rand.nextInt(2) == 0) {
					scheme += String.valueOf(rand.nextInt(10));
				}
				else {
					scheme += ((char) rand.nextInt(11)+35);
				}
			}
			//otherwise make it uppercase if 2 33%
			else if (addOnChoice == 2){
				scheme += ((char) (rand.nextInt(26)+65));
			}
			//if 3 make it uppercase 33%
			else {
				scheme += ((char) (rand.nextInt(26)+97));
			}
			i++;
		}
		
		return scheme;
	}
	
	
	public String generatePath(Random rand) {
		
		//randomly pick how long the path can be 1-20 chars
		int numberOfChars = rand.nextInt(20) + 1;
		String path = "";
		int i = 0;
		
		//go through and add each randomly generated char to the path
		while (i < numberOfChars) {
			
			//generate number 1-10 as we have 5 choices to add to it - number, letter( upper, lower), / or symbol
			//so 60% chance combined letters (1-6) 20% number (7 & 8), 10% / (9), and 10% symbol (10)
			int addOn = rand.nextInt(10) + 1;
			
			//if we get a number between 1-6 then make it a letter 60%
			if(addOn <= 6) {
				
				//randomly choose upper or lower case letter
				if(rand.nextInt(2) == 0) {
					path += ((char) (rand.nextInt(26)+65));				
				}
				else {
					path += ((char) (rand.nextInt(26)+97));
				}
				
			}
			
			//20% chance to pick a number
			else if(addOn == 7 || addOn == 8) {
				path += String.valueOf(rand.nextInt(10));
			}
			//if we get 9 make it / with a 10%
			else if (addOn == 9) {
				path += "/";
			}
			//if we get 10 make it a symbol - 10% chance
			//NOTE: Include ? in the symbols after debating it. Decided best to include to make
			//sure it fails (as ? is symbol for queries and it is unlikely a well-formed query
			//will be formed from the path random generator
			else {
				path += ((char) (rand.nextInt(11)+35));
			}
			i++;
		}
		
		return path;
	}
	
	
	public String generateSchemeHostLink(Random rand) {
		
		//Should be 3 chars :// if correct going from 0 (nothing) to 4 chars to test boundaries
		int numberOfChars = rand.nextInt(5);
		String link = "";

		int i = 0;
		
		
		//go through and add each randomly generated char to the scheme/host link (the ://)
		while(i < numberOfChars) {	
			
			//there should be 2 / and 1 : in a proper :// so giving 66% chance for / and 33% for :
			int addOnChoice = rand.nextInt(3);
			
			//if choice is 0 or 1 add / (66%)
			if(addOnChoice < 2) {
				link += "/";
			}
			
			//otherwise add : (33%)
			else {
				link += ":";
			}
			
			i++;

		}
		
		return link;
		
	}
public String generateQuery(Random rand) {
		
		//randomly pick how long the query can be 1-20 chars (want to account for long queries)
		int numberOfChars = rand.nextInt(20) + 1;
		String query = "";
		int i = 0;
		
		//Give a 10% chance for the query to be missing the question mark in front
		int questionMarkInFront = rand.nextInt(10) + 1;
		if(questionMarkInFront != 1){
			query+="?";
		}
		
		//go through and add each randomly generated char to the query
		while (i < numberOfChars) {
			
			//generate number 1-10 as we have 5 choices to add to it - number, letter( upper, lower), & or =
			//giving letters 60% chance combined (randomly chosen upper or lower) then 20% number, and 10% for & and 10% for =
			int addOn = rand.nextInt(10) + 1;
			
			//if we get a number between 1-6 then make it a letter 60%
			if(addOn <= 6) {
				
				//randomly choose upper or lower case letter
				if(rand.nextInt(2) == 0) {
					query += ((char) (rand.nextInt(26)+65));				
				}
				else {
					query += ((char) (rand.nextInt(26)+97));
				}
				
			}
			
			//20% chance to pick a number
			else if(addOn == 7 || addOn == 8) {
				query += String.valueOf(rand.nextInt(10));
			}
			//if we get 9 make it & with a 10%
			else if (addOn == 9) {
				query += "&";
			}
			//if we get 10 make it a = - 10% chance
			else {
				query += "=";
			}
			i++;
		}
		
		return query;
	}

public String generateHost(Random rand) {
	
	//randomly pick how long the query can be 1-20 chars (want to account for long hosts)
	int numberOfChars = rand.nextInt(20) + 1;
	String host = "";
	int i = 0;
	

	//go through and add each randomly generated char to the host
	while (i < numberOfChars) {
		
		//generate number 1-4 as we have 3 choices to add to it number, letters, and .
		//however letters we want to appear slightly more often as in normal cases
		//letters dominate hosts...so 50% chance of letter and 25% for number and 25% for .
		int addOn = rand.nextInt(4) + 1;
		
		//if we get a number 1 make it an uppercase character that gets added
		//using ASCII codes
		if(addOn == 1) {
			host += ((char) (rand.nextInt(26)+65));
		}
		//otherwise if 2 make it lower case
		else if (addOn == 2) {
			host += ((char) (rand.nextInt(26)+97));
		}
		//If we get a 3 make it a number
		else if (addOn == 3) {
			host += String.valueOf(rand.nextInt(10));
		}
		//If we get a 4 make it a .
		else {
			host += ".";
		}
		i++;
	}
	
	return host;
}
	
	@Test
	public void testIsValid() throws IOException {
		
		//set up urlVal to be used in the tests
	     UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

		
	     //Create file for writing results (results will also be written to console
	      PrintWriter fileWriter;
	      fileWriter = new PrintWriter("randomTestResults.txt", "UTF-8");
	      
	  	
	    //create new random generator to use in randomly generating the urls
		Random rand = new Random(); 
		
		//variables to count the total amount of fails
		int schemeAndHostFailAmt = 0;
		int portNumFailAmt = 0;
		int pathFailAmt = 0;
		int queryFailAmt = 0;
		
		//variables to count when only specific areas failed -> useful to breakdown what's happening
		int onlyPathFail = 0;
		int onlyQueryFail = 0;
		int onlyPortFail = 0;
		

		//variables to keep track of what items were added to the url
		int addedQueryAmt = 0;
		int addedPathAmt = 0;
		int addedPortAmt = 0;
		
		
		//variable to keep track whether or not the oracle was able to create the url
		boolean urlValid = true;
	
		
		int i = 0;
		
		//Runs through 100,000 random tests random urls
		while (i < 100000) {
			
			//checks to see if the path, query, or port were added
			boolean addedPath = false;
			boolean addedQuery = false;
			boolean addedPort = false;
			
			//keeps track of which ones pass and which fails so
			//results can be more clear on what exactly if failing
			boolean schemeHostPass = true;
			boolean pathQueryPass = true;
			
			String currentUrl = "";
			
			//Add the scheme (note scheme has 10% chance of being empty)
			String scheme = generateScheme(rand);
			currentUrl += scheme;
			
			//add the host scheme (25% chance empty, 50% chance correct amount of characters and 25% chance 1 too many chars)
			currentUrl += generateSchemeHostLink(rand);
			
			//get the host name
			String host = generateHost(rand);
			currentUrl += host;
						
			//try the basic scheme + host name url
			//if the new url cannot be created then urlValid is set to false
			try {
				URL myURL = new URL(currentUrl);
			} 
			catch (MalformedURLException e) {
				urlValid = false;
			}
			
			//Check by calling the function to see if they match (they should!) if they do, the test passes
			if(urlValid != urlVal.isValid(currentUrl)) {
				schemeAndHostFailAmt++;
				schemeHostPass = false;
			}
			
			
			//reset urlValid
			urlValid = true;
			
			
			//variables to hold path, query and combined
			String path = "";
			String query = "";
			String pathAndQuery = "";
		
			//75% of the time generate a path
			if(rand.nextInt(4) != 0) {
				addedPathAmt++;
				addedPath = true;
				path = generatePath(rand);
			}
			
			//75% of the time generate a query
			if(rand.nextInt(4) != 0) {
				addedQueryAmt++;
				addedQuery = true;
				query = generateQuery(rand);
			}
			
			//combine the path and query in proper form oracle needs
			pathAndQuery = path + query;
			
				//if the new url cannot be created then urlValid is set to false
				try {
					URL myURL = new URL(scheme, host, pathAndQuery);
				} 
				catch (MalformedURLException e) {
					urlValid = false;
				}
				
				//check the oracle vs. the function we are testing
				if(urlValid != urlVal.isValid(scheme + "://" + host + "/" + pathAndQuery)) {
					
					pathQueryPass = false;
					
					//if we added the path  and there was a fail, then increase fail amount
					if(addedPath) {
						pathFailAmt++;
						
						//add breakdown so we can see when just the path failed (no query)
						if(schemeHostPass && !addedQuery) {
							onlyPathFail++;
						}
					}
					
					
					//if we added the query and there was a fail, then increase fail amount
					if(addedQuery) {
						queryFailAmt++;
						
						//add breakdown so we can see when just the query failed (no path)
						if(schemeHostPass && !addedPath) {
							onlyQueryFail++;
						}
					}
				}
				
				//reset urlValid
				urlValid = true;
				
				//doesn't really matter what port is set to as it will be reset to something in 
				//below if or won't be used...
				int port = -1;
				
				//75% of the time generate a port
				if(rand.nextInt(4) != 0) {
					addedPortAmt++;
					addedPort = true;
					port = generatePort(rand);
				}
				
				//Only if we added a port try and make a url with one
				if(addedPort) {
					try {
						URL myURL = new URL(scheme, host, port, pathAndQuery);
					} 
					catch (MalformedURLException e) {
						urlValid = false;
					}
					
					//if the oracle throws an error then increment portNumFailAmt
					if(urlValid != urlVal.isValid(scheme + "://" + host + ":" 
					+ String.valueOf(port) + "/" + pathAndQuery)) {
						portNumFailAmt++;
						
						//however as port is coming so late in the testing, also keep track
						//of whether this is actually port failing or scheme/path/query failing beforehand
						if(schemeHostPass && pathQueryPass) {
							onlyPortFail++;
						}
					}
				}

			i++;
		}
		
		//write results to the file
	      fileWriter.println("Completed " + String.valueOf(i) + " Random Tests\n");
	      fileWriter.println("Scheme and Host fails: " + String.valueOf(schemeAndHostFailAmt));
	      fileWriter.println("Added query" + String.valueOf(addedQueryAmt) +" times. Query fails: " 
	      + String.valueOf(queryFailAmt) + " Only query fails: " + String.valueOf(onlyQueryFail) );
	      fileWriter.println("Added path" + String.valueOf(addedPathAmt) +" times. Path fails: " 
	      + String.valueOf(pathFailAmt) + " Only path fails: " + String.valueOf(onlyPathFail));
	      fileWriter.println("Added port number" + String.valueOf(addedPortAmt) +" times. Port fails: " 
	      + String.valueOf(portNumFailAmt) + " Only port fails: " + String.valueOf(onlyPortFail));
	      fileWriter.close();
	      
	      
	      //write out the results to the console
	      System.out.println("Completed " + String.valueOf(i) + " Random Tests\n"
	    		  + "Scheme and Host fails: " + String.valueOf(schemeAndHostFailAmt)
	    		  + "\nAdded query " + String.valueOf(addedQueryAmt) +" times. Query fails: " 
	    		  + String.valueOf(queryFailAmt) + " Only query fails: " + String.valueOf(onlyQueryFail) 
	    		  + "\nAdded path " + String.valueOf(addedPathAmt) + " times. Path fails: " 
	    		  + String.valueOf(pathFailAmt) + " Only path fails: " + String.valueOf(onlyPathFail)
	    		  + "\nAdded port number " + String.valueOf(addedPortAmt) + " times. Port fails: " 
	    		  + String.valueOf(portNumFailAmt) + " Only port fails: " + String.valueOf(onlyPortFail));
	      
	}
}
