#!/usr/bin/expect -f

set timeout -1

spawn ./menu

expect "Enter a number that corresponds to your choice > "
send -- "2\r"

expect "Enter a number that corresponds to your choice > "
send -- "1\r"


# 7933 -> stock for product with id = S10_1678
expect "   | Order number"
expect "01 | 10167"
expect "02 | 10248"
expect "03 | 10260"
expect "04 | 10262"
expect "05 | 10334"
expect "06 | 10401"
expect "07 | 10407"
expect "08 | 10414"
expect "09 | 10420"
expect "10 | 10421"
expect "11 | 10422"
expect "12 | 10423"
expect "13 | 10424"
expect "14 | 10425"

puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect "Enter a number that corresponds to your choice > "
send -- "4\r"

expect eof
