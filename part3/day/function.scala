def day(date: String) = {
  var ret = ""
  var hyphens = 0
  var count = 0

  for( char <- date) {
    if(char == '-') {
        hyphens += 1
        if(hyphens == 2) {
            ret = date.substring(count+1, count+3)
        }
    } 
    count += 1
  }
  ret.toInt
}