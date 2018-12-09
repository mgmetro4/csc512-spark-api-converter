def last_day(date: String) = {
  var ret = ""
  var count = 0
  var month = 0
  var year = 0
  var char = date.substring(0, 1)

 while( char != "-") {
    count +=1
    char = date.substring(count, count+1)
  }
  month = (date.substring(count+1, count+3)).toInt

  //find last day from "counting months on your knuckles" trick
  if(month < 8) {
    if(month%2 != 0) {
      ret = "31"
    }
    else if (month == 2) { //FEBURARY - LEAP YEAR??
      count = 0
      char = date.substring(0,1)
       while( char != "-") {
        count += 1
        char = date.substring(count, count+1)
        }
        year = (date.substring(count-4, count)).toInt
      if((year % 4 == 0) && !( (year%100 == 0) && (year%400 != 0) )  )  {
        ret = "29"
      }
      else {
        ret = "28"
      }
    }
    else {
      ret = "30"
    }

  }
  
  else {
    if (month%2 == 0) {
      ret = "31"
    }
    else {
      ret = "30"
    }
  }
  date.substring(0,8) + ret
  
}