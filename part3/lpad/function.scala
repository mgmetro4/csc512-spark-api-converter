def lpad(str: String, len: Int, pad: String) = {
  
  var ret = ""
  var padding = ""
  var i = str.size
  var diff = 0

  if( len <= str.size ) {
    ret = str.substring(0, len)
  } 
  else {
    ret += str
    while( i < len) {
        diff = len - i
        if(diff < pad.size) {
          padding += pad.substring(0, diff)
          i = padding.size + ret.size
        }
        else {
          padding += pad
          i = ret.size + padding.size
        }
    }
    ret = padding + ret
  }
  ret 
}