# Text File Analysis
This program is designed to analyze a text file and generate statistics. For instance, we want to determine word frequencies, frequencies of words with a certain length, etc.
It achieves this through the construction of a hash table to store encountered words.

# Features
* Insert Words: Insert text in the program for analysis.
```
insert <text>
```
* Display Hash Table: View the contents of the entire hash table.
```
print
```
* Display Words by Starting Letter and Length: Retrieve words starting with a specific letter and of a certain length.
```
print <starting_letter> <length>
```
* Display Words with Maximum Occurrences: Display words appearing a maximum number of times.
```
print <max_nr_of_occ>
```

# Example
The result of running:
```
insert E un balon albastru.
insert Anca are un balon albastru.
insert acasa e bine
```
![example](https://iili.io/J1bGmFe.png)
