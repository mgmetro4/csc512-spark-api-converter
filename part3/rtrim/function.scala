def rtrim(str: String) = {
    
  var (s, e) = (0, str.size)
  while (e > s && str(e-1) == ' ') e -= 1
  str.substring(s, e)
}

