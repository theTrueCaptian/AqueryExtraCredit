	Welcome to your aquery extra credit homework.

1. Go to kx.com and download q for your machine.

2. Place it in the src subdirectory of this directory.

3. From src directory, make sure q executes.
Just type
q
on the command line and see that you get something that begins
with KDB....:

4. Change permissions to allow execution of myrun
chmod +x myrun

5. Still in src directory, type
./myrun homework

It should return:

`:ticks.csv
`.
`ticks
`ticks
ID   maxavgprice
----------------
1000 182.6667   
1001 172.3333   
1002 178.6667   
1003 168.3333   
+`ID`maxavgprice!(1000 1001 1002 1003i;182.6667 172.3333 178.6667 168.3333)


6. Now modify that file homework.a to do your homework and submit to the grader.

Troubleshooting
- This workflow was tested on energon1 and worked accordingly. Note that the binary
for ./a2q, which is required, was also compiled on energon1. If you plan on using
on a different machine. Please call rebuild by calling (from the src directory)

make

For any issues with building/executing, please feel free to reach out to jose.cambronero@cs.nyu.edu


General aquery information
- Currently the aquery implementation available here does not permit for nested queries
 However, you can make use of temporary tables as follows
 WITH
   temp_table1(col1, col2) AS (SELECT ... )
   temp_table2 AS (SELECT ...)
   SELECT * FROM temp_table1, temp_table2

- Note that aquery allows nested arrables, you can remove nesting by using the FLATTEN built-in

