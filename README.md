# XPS

[![Build Status](https://travis-ci.org/wizjin/xps.svg?branch=master)](https://travis-ci.org/wizjin/xps)
[![codecov](https://codecov.io/gh/wizjin/xps/branch/master/graph/badge.svg)](https://codecov.io/gh/wizjin/xps)
[![CocoaPods Compatible](https://img.shields.io/cocoapods/v/XPS.svg)](https://img.shields.io/cocoapods/v/XPS.svg)
[![Carthage Compatible](https://img.shields.io/badge/Carthage-compatible-4BC51D.svg?style=flat)](https://github.com/Carthage/Carthage)
[![Platform](https://img.shields.io/cocoapods/p/XPS.svg?style=flat)](http://cocoadocs.org/docsets/XPS)

XPS is a light-weight X proxy system library for iOS and macOS.

## Installation
XPS supports multiple methods for installing the library in a project.

### CocoaPods

[CocoaPods](http://cocoapods.org) is a dependency manager for Objective-C, which automates and simplifies the process of using 3rd-party libraries like XPS in your projects. You can install it with the following command:

```bash
$ gem install cocoapods
```

> CocoaPods 1.5.0+ is required to build XPS 0.1.0+.

#### Podfile

To integrate XPS into your Xcode project using CocoaPods, specify it in your `Podfile`:

```ruby
source 'https://github.com/CocoaPods/Specs.git'
platform :ios, '10.0'

target 'TargetName' do
pod 'XPS', '~> 0.1.0'
end
```

Then, run the following command:

```bash
$ pod install
```

### Carthage

[Carthage](https://github.com/Carthage/Carthage) is a decentralized dependency manager that builds your dependencies and provides you with binary frameworks.

You can install Carthage with [Homebrew](http://brew.sh/) using the following command:

```bash
$ brew update
$ brew install carthage
```

To integrate XPS into your Xcode project using Carthage, specify it in your `Cartfile`:

```ogdl
github "wizjin/xps" ~> 0.1.0
```

Run `carthage` to build the framework and drag the built `XPS.framework` into your Xcode project.

## License

XPS is released under the MIT license. See [LICENSE](https://github.com/wizjin/xps/blob/master/LICENSE) for details.
