find . -type d -exec printf "-I%s\n" {} \; > .syntastic_config
