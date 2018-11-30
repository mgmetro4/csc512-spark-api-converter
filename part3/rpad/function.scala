def rpad(str: String, len: Int, pad: String) = {
  
  var ret = ""
  var i = str.size
  var diff = 0

  if( len <= str.size ) {
    ret = str.substring(0, len-1)
  } 
  else {
    ret += str
    while( i < len) {
        diff = len - i
        if(diff < pad.size) {
          ret += pad.substring(0, diff)
          i = ret.size
        }
        else {
          ret += pad
          i = ret.size
        }
    }
  }
  ret
}