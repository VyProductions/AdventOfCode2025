all: clean Part1 Part2
	@clear
	@if [ -e "Part1.exe" ]; then \
		printf "Part 1\n" > actual.txt; \
		./Part1 | tr -d '\r' >> actual.txt; \
	else \
		printf "Part 1 Not Found. See 'compile.txt'." > actual.txt; \
	fi
	@if [ -e "Part2.exe" ]; then \
		printf "Part 2\n" >> actual.txt; \
		./Part2 | tr -d '\r' >> actual.txt; \
	else \
		printf "Part 2 Not Found. See 'compile.txt'." >> actual.txt; \
	fi

	@xxd actual.txt
	@xxd expected.txt
	@diff -y --color actual.txt expected.txt

Part1:
	-@g++ Part1.cpp -o Part1 > compile.txt 2>&1

Part2:
	@printf " ------ " >> compile.txt
	-@g++ Part2.cpp -o Part2 >> compile.txt 2>&1

clean:
	@rm -f *.exe compile.txt actual.txt
