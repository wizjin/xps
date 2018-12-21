VERSION=$(shell grep -m 1 version XPS.podspec | cut -f2 -d"'")

test:
	@clang -coverage -O0 tests/test.c -o test
	@./test
	@gcov test.c

release:
	@echo release version ${VERSION}
	@sed -i '' 's/>[0-9]*\.[0-9]*\.[0-9]*</>${VERSION}</g' XPS-iOS/Info.plist
	@sed -i '' 's/>[0-9]*\.[0-9]*\.[0-9]*</>${VERSION}</g' XPS-macOS/Info.plist
	@sed -i '' 's/"[0-9]*\.[0-9]*\.[0-9]*"/"${VERSION}"/g' libxps/libxps.c

.PHONY: test release
