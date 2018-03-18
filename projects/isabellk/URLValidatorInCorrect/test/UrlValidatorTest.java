

import junit.framework.TestCase;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
    //You can use this function to implement your manual testing	   
	   
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
   
   public void testIsValid()
   {
	   //You can use this function for programming based testing

   }
   


}
