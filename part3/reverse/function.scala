def reverse(str: String) = {
  
  var ret = ""

  for( i <- str.size until 0 by -1) {
    ret += str.substring(i-1, i)
  }
  ret
}

