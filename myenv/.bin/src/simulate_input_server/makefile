target := simulate_input_server

${target}: ${target}.c
	g++ $< -o $@
	chown root:root $@
	chmod +s $@
clean:
	rm ${target}
