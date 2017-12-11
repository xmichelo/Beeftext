# Beeftext

Beeftext is a text snippet management tool for Windows, inspired by [TextExpander].

TextExpander is a great program, packed with features. To to keep the company profitable, 
[Smile] had to switch to a subscription-based model in 2016. And for people who only need the
basic features of TextExpander, the monthly price is way too high.

Beeftext aims to be the best possible alternative to TextExpander. Beeftext is an open-source project 
written in C++ using the [Qt] framework.

## Author

The project is developed and managed by [Xavier Michelon].

## License

This project is licensed under the MIT License - see the [LICENSE] file for details.

## Documentation
### End User

No end-user documentation is available at the moment.

### Developer

Developer documentation can be extracted using the [Doxygen] documentation tool.

## Building Beeftext

In order to build Beeftext from sources you will need:

- The [Beeftext] source code.
- [Visual Studio 2017]. The Community Edition is available for free.
- The [Qt] framework. Beeftext is generally built using the latest version of the library. Beeftext uses the 32bit version compiled for the most recent version of Visual Studio.
- The [Qt Visual Studio Tools]. This add-on can be installed from the `Extensions and Updates` tool built into Visual Studio.
- The [XMiLib] library source code. The library root folder must be located next to the Beeftext source code root folder.
- If you plan to generate the installer for Beeftext, you will also need [NSIS].

Detailed build instructions are not available at the moment.

[TextExpander]: https://textexpander.com
[Smile]: https://smilesoftware.com/
[Qt]: https://www.qt.io/developers/
[Xavier Michelon]: https://x-mi.com
[LICENSE]:LICENSE
[Doxygen]: http://www.stack.nl/~dimitri/doxygen/
[Beeftext]: https://github.com/xmichelo/Beeftext
[Visual Studio 2017]: https://www.visualstudio.com/vs/community/
[Qt Visual Studio Tools]: https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123
[XMiLib]: https://github.com/xmichelo/XMiLib
[NSIS]: http://nsis.sourceforge.net/Main_Page