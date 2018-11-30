def repeat(str: String, n:Int) = {
    var ret = ""
    var count = n

    if(n <= 0) {
        ret = ""
    }

    else {
        while(count > 0) {
            ret += str
            count -= 1
        }
    }
    ret
}

