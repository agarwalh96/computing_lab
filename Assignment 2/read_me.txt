
Question 1

Notes & Assumptions:
Final tree containing the reduced BDD has missing indices in between from where the nodes have been removed. The array nodelist is not continuous in that sense, since it is not mentioned to create a new tree with the nodes present serially.
The reduced BDD is being written to a file named reduced_BDD.txt and also printed to stdout in the below format:
	array_index		data	left	right	 level
The index is being printed (not the format in Lab Test 2) since the elements are not present serially.
The root index of this reduced tree is also printed to stdout.