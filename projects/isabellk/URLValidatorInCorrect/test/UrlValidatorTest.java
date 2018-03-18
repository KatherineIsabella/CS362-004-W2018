

import junit.framework.TestCase;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;



public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
	// create new validator object
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

	   // manually create lists of sample valid & invalid URLs
	   String[] validUrls = {
	      "https://www.google.com",
	      "http://www.google.com",
	      "http://google.com",
	      "google.com",
	      "google.com/",
	      "https://google.com/",
	      "www.google.com",
	      "www.google.com?msg=hello",
	      "www.google.com/?msg=hello&hi=msg",
	      "www.google.com/?msg=hi/",
	      "http://google.com/?msg=hello",
	      "google.com:64234",
	      "www.google.com:240",
	      "255:255:255:255",
	      "entertainment720.com"
	   };

	   String[] invalidUrls = {
	      "google/com",
	      "!google.com",
	      "www.!google.com",
	      "wwwwwww.google.com",
	      "httttps://www.google.com",
	      "google.com?msg=hello?msg=hi?msg=sup",
	      "//www.google.com",
	      "http:google.com",
	      "google.com/////",
	      "https://www.google.com:hello?there",
	      "https://google",
	      "http://google.com:::",
	      "goo..co:25",
	      "0.0.0.0.0.0.0.0.00.0.0.0.0.0",
	      "239595.333"
	   };

	   // check valid URLs
	   // passing test (isValid true) prints a .
	   for (int i = 0; i < validUrls.length; i++) {
	      if (urlVal.isValid(validUrls[i])) {
	         System.out.println(".");
	      } else {
	         System.out.println("x");
	      }
	   }

	   // check invalid URLs
	   // passing test (isValid false) print a .
	   for (int i = 0; i < invalidUrls.length; i++) {
	      if (!(urlVal.isValid(invalidUrls[i]))) {
	         System.out.println(".");
	      } else {
	         System.out.println("x");
	      }         
	   }
	   
   }
   
   
   /*First focusing on length/size of the urls - specifically domain*/
   public void testYourFirstPartition()
   {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   
	   /*Testing a url that is null/empty should fail*/
	   if(urlVal.isValid("")){
		  System.out.println("Test failed: url is not allowed to be empty");
	   }
	   
	   /*Testing a url with only one character should fail*/
	  if(urlVal.isValid("g")) {
		  System.out.println("Test failed: url cannot be one character");
	  }
	   
	   /*Testing a url with the a normal amount of characters should pass*/
	  /*Normal amount = anything that holds all the necessary pieces (domain name, etc.) and is less than 2083*/
	   if(!(urlVal.isValid("http://google.com"))){
			  System.out.println("Test failed: Url length can be a normal amount");
	   }
	   
	   /*Testing a domain of length 254 should fail as there is a max length specified of 253 so this should fail*/
	  if(urlVal.isValid("http://googletwohundredeightyfourcharacters"
	  		+ "longdomainnameleadstoareallylongurlgoogletwohundredeightyfour"
	  		+ "characterslongdomainnameleadstoareallylongurlgoogletwohundredeighty"
	  		+ "fourcharacterslongdomainnameleadstoareallylongurlgoogletwohundredeighty"
	  		+ "fourcharacterslongdomainnameleadstoareallylon.com")) {
		  System.out.println("Test failed: Domain length has a max of 253");
	  }
   }
   
   /*Parition based on schemes/protocols*/
   public void testYourSecondPartition(){
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   
	   /*NOTE TEST FAILS AND CAUSES EXCEPTION
		Test a valid, default scheme (http, https) 
	   if(!(urlVal.isValid("https://google.com"))) {
			  System.out.println("Test failed: url cannot be one character");
		}*/
		   
	   
	  /*NOTE - BELOW TEST FAILS AND CAUSES EXCEPTION
		 Test a valid non-default scheme (abc, file) 
	   if(!(urlVal.isValid("file://google.com"))) {
			  System.out.println("Test failed: All schemes, not just default, are allowed");
		  }*/
		  

	   /*Test an invalid scheme - add disallowed symbols (according to research . - and + are all allowed*/
	   if(urlVal.isValid("http$://google.com")) {
			  System.out.println("Test failed: Scheme cannot have symbols that aren't valid");
		}
		
	/*	NOTE - BELOW TEST FAILS AND CAUSES EXCEPTION
		Test a valid scheme with allowed symbols
		  if(!(urlVal.isValid("http.h://google.com"))) {
			  System.out.println("Test failed: Scheme can have symbols that are valid");
		}*/
		
		/*Test an invalid scheme (missing part of the ://)*/
	   if((urlVal.isValid("https//google.com"))) {
			  System.out.println("Test failed: Scheme needs to have :// before the domain");
		  }
		  
		/*Test an empty scheme with onyl ://*/
		 if(!(urlVal.isValid("://google.com"))) {
			  System.out.println("Test failed: Scheme cannot just have ://");
			  }
   }
   
   /*Partition and test the Port Number*/
   public void testYourThirdPartition(){
   
   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
   
   
   /*Test a valid url with no port*/
   if(!(urlVal.isValid("http://google.com"))) {
		System.out.println("Test failed: Url can be valid without a port");
	}
   
   /*Test a valid url with a port that is between 0 - 65535*/
      if(!(urlVal.isValid("http://google.com:0"))) {
		System.out.println("Test failed: Valid port numbers are between 0 - 65535");
	}
   
   
   /*Test an invalid url with a port that is a negative number*/
   if((urlVal.isValid("http://google.com:-5"))) {
		System.out.println("Test failed: Valid port numbers cannot be negative");
	}
   
   /*Test an invalid url with a port that is a greater than 65535*/
      if((urlVal.isValid("http://google.com:65536"))) {
		System.out.println("Test failed: Valid port numbers cannot be greater than 65535");
	}

 }
   /*Fourth parition focusing on paths and queries */
   public void testYourFourthPartition(){
   
    UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
   
   /*Test a valid url without a path or query*/
    if(!(urlVal.isValid("http://google.com"))) {
		System.out.println("Test failed: A url does not require a path or a query");
	}
   
   /*Test a valid url with just a path*/
    if(!(urlVal.isValid("http://google.com/path"))) {
		System.out.println("Test failed: A url can have a path");
	}
	
   /*Test a valid url with just a query*/
    if(!(urlVal.isValid("http://google.com/?path=true"))) {
		System.out.println("Test failed: A url can have a query");
	}
	
   /*Test a valid url with both a query and path*/
   if(!(urlVal.isValid("http://google.com/path/?path=true"))) {
		System.out.println("Test failed: A url can have both a path and a query");
	}
	
   /*Test an invalid url with a path formated wrong (no query)*/
    if((urlVal.isValid("http://google.com//path/"))) {
		System.out.println("Test failed: A path must be properly formatted");
	}
	
   /*Test an invalid url with a query formated wrong (no path)*/
   if((urlVal.isValid("http://google.com/?query =bad"))) {
		System.out.println("Test failed: A query must be properly formatted");
	}
 }
   
   public void testIsValid() throws FileNotFoundException, UnsupportedEncodingException
   {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   PrintWriter writer_true = new PrintWriter("correct-true.txt", "UTF-8");
	   PrintWriter writer_false = new PrintWriter("correct-false.txt", "UTF-8");
	   PrintWriter writer_bad = new PrintWriter("incorrect.txt", "UTF-8");
	   	   
	   assertTrue(urlVal.isValid("https://www.google.com"));
	   assertTrue(urlVal.isValid("https://www.bing.com"));
	   //assertTrue(urlVal.isValid("https://www.google.com/~lewj"));
	   
	   assertTrue(testUrlScheme[0].valid);
	   assertEquals(testUrlScheme[0].item, "http://");
	   
	   for (int i = 0; i < testUrlScheme.length; i++) {
		   for (int j = 0; j < testUrlAuthority.length; j++) {
			   for (int k = 0; k < testUrlPort.length; k++) {
				   for (int m = 0; m < testPath.length; m++) {
					   for (int n = 0; n < testUrlQuery.length; n++) {
						   StringBuilder testUrl = new StringBuilder();
						   boolean urlValid = true;
						   
						   testUrl.append(testUrlScheme[i].item);
						   urlValid = urlValid && testUrlScheme[i].valid;
						   
						   testUrl.append(testUrlAuthority[j].item);
						   urlValid = urlValid && testUrlAuthority[j].valid;
						   
						   testUrl.append(testUrlPort[k].item);
						   urlValid = urlValid && testUrlPort[k].valid;
						   
						   testUrl.append(testPath[m].item);
						   urlValid = urlValid && testPath[m].valid;
						   
						   testUrl.append(testUrlQuery[n].item);
						   urlValid = urlValid && testUrlQuery[n].valid;
						   
						   if (urlVal.isValid(testUrl.toString()) == urlValid && urlValid == true) {
							   writer_true.println(testUrl);
						   }
						   else if (urlVal.isValid(testUrl.toString()) == urlValid && urlValid == false) {
							   writer_false.println(testUrl);
						   }
						   else {
							   writer_bad.print(testUrl);
							   writer_bad.print(" | expected: ");
							   writer_bad.print(urlValid);
							   writer_bad.print(", urlValidator: ");
							   writer_bad.println(urlVal.isValid(testUrl.toString()));
						   }
						  
						 
					   }
				   }
			   }
		   }
	   }
	  
	   writer_true.close();
	   writer_false.close();
	   writer_bad.close();
	   
   }






   //-------------------- Test data for creating a composite URL
   /**
    * The data given below approximates the 4 parts of a URL
    * <scheme>://<authority><path>?<query> except that the port number
    * is broken out of authority to increase the number of permutations.
    * A complete URL is composed of a scheme+authority+port+path+query,
    * all of which must be individually valid for the entire URL to be considered
    * valid.
    */
   ResultPair[] testUrlScheme = {new ResultPair("http://", true),
                               new ResultPair("ftp://", true),
                               new ResultPair("h3t://", true),
                               new ResultPair("3ht://", false),
                               new ResultPair("http:/", false),
                               new ResultPair("http:", false),
                               new ResultPair("http/", false),
                               new ResultPair("://", false),
                               new ResultPair("", true)};

   ResultPair[] testUrlAuthority = {new ResultPair("www.google.com", true),
                                  new ResultPair("go.com", true),
                                  new ResultPair("go.au", true),
                                  new ResultPair("0.0.0.0", true),
                                  new ResultPair("255.255.255.255", true),
                                  new ResultPair("256.256.256.256", false),
                                  new ResultPair("255.com", true),
                                  new ResultPair("1.2.3.4.5", false),
                                  new ResultPair("1.2.3.4.", false),
                                  new ResultPair("1.2.3", false),
                                  new ResultPair(".1.2.3.4", false),
                                  new ResultPair("go.a", false),
                                 new ResultPair("go.a1a", false),
                                  new ResultPair("go.1aa", false),
                                  new ResultPair("aaa.", false),
                                  new ResultPair(".aaa", false),
                                  new ResultPair("aaa", false),
                                  new ResultPair("", false)
   };
   ResultPair[] testUrlPort = {new ResultPair(":80", true),
                             new ResultPair(":65535", true),
                             new ResultPair(":0", true),
                             new ResultPair("", true),
                             new ResultPair(":-1", false),
                            new ResultPair(":65636",false),
                             new ResultPair(":65a", false)
   };
   ResultPair[] testPath = {new ResultPair("/test1", true),
                          new ResultPair("/t123", true),
                          new ResultPair("/$23", true),
                          new ResultPair("/..", false),
                          new ResultPair("/../", false),
                          new ResultPair("/test1/", true),
                          new ResultPair("", true),
                          new ResultPair("/test1/file", true),
                          new ResultPair("/..//file", false),
                          new ResultPair("/test1//file", false),
                          new ResultPair("/~mpython", true)
   };
   //Test allow2slash, noFragment
   ResultPair[] testUrlPathOptions = {new ResultPair("/test1", true),
                                    new ResultPair("/t123", true),
                                    new ResultPair("/$23", true),
                                    new ResultPair("/..", false),
                                    new ResultPair("/../", false),
                                    new ResultPair("/test1/", true),
                                    new ResultPair("/#", false),
                                    new ResultPair("", true),
                                    new ResultPair("/test1/file", true),
                                    new ResultPair("/t123/file", true),
                                    new ResultPair("/$23/file", true),
                                    new ResultPair("/../file", false),
                                    new ResultPair("/..//file", false),
                                    new ResultPair("/test1//file", true),
                                    new ResultPair("/#/file", false)
   };

   ResultPair[] testUrlQuery = {new ResultPair("?action=view", true),
                              new ResultPair("?action=edit&mode=up", true),
                              new ResultPair("", true)
   };

   Object[] testUrlParts = {testUrlScheme, testUrlAuthority, testUrlPort, testPath, testUrlQuery};
   Object[] testUrlPartsOptions = {testUrlScheme, testUrlAuthority, testUrlPort, testUrlPathOptions, testUrlQuery};
   int[] testPartsIndex = {0, 0, 0, 0, 0};

   //---------------- Test data for individual url parts ----------------
   ResultPair[] testScheme = {new ResultPair("http", true),
                            new ResultPair("ftp", false),
                            new ResultPair("httpd", false),
                            new ResultPair("telnet", false)};

   }

