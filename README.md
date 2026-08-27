# Abbrevio

![Abbrevio](https://raw.githubusercontent.com/wiki/CanScribe/Abbrevio/assets/images/Screenshot.png)

Abbrevio is a text snippet management tool for Windows, inspired by [BeefText].

> [!important]  
> Abbrevio is now in maintenance mode. No new feature will be added, but fixes for major issues will be provided.

TextExpander is a great program, packed with features. To keep the company profitable, 
[Smile] had to switch to a subscription-based model in 2016. And for people who only need the
basic features of TextExpander, the monthly price is way too high.

Abbrevio aims to be the best possible alternative to TextExpander. Abbrevio is an open-source project 
written in C++ using the [Qt] framework.

## Contributors

The project is developed and managed by [Xavier Michelon]. The project is not looking for contributors and all pull requests will be rejected.

Contributors for translations are listed in the [dedicated wiki page](https://github.com/CanScribe/Abbrevio/wiki/User-provided-translations).

The [abbrevio.org] website uses a template provided by [HTML5 UP].

## License

This project is licensed under the [MIT License].

Abbrevio uses the [Qt] framework under the [GNU Lesser General Public License v3].

Abbrevio uses the [OpenSSL] library under [OpenSSL and SSLeay licenses].

Abbrevio uses the [emojilib](https://github.com/muan/emojilib) library under the [MIT License].

## Documentation
### End User

The end user documentation is available as a [wiki](https://github.com/CanScribe/Abbrevio/wiki) on the GitHub project page.

### Developer

Developer documentation can be extracted using the [Doxygen] documentation tool.

## Building Abbrevio

In order to build Abbrevio from sources you will need:

- The [Abbrevio] source code. The git repository includes the [XMiLib] library as a submodule.
- [Visual Studio 2022]. The Community Edition is available for free.
- The [Qt] framework. Abbrevio is generally built using the latest version of the library. Abbrevio uses the 64-bit version compiled for the most recent version of Visual Studio.
- The [Qt Visual Studio Tools]. This add-on can be installed from the `Extensions and Updates` tool built into Visual Studio.
- If you plan to generate the installer for Abbrevio, you will also need [NSIS].

Detailed build instructions are not available at the moment.

[TextExpander]: https://textexpander.com
[Smile]: https://smilesoftware.com/
[Qt]: https://www.qt.io/developers/
[Xavier Michelon]: https://michelon.ch
[abbrevio.org]: https://abbrevio.org
[HTML5 UP]: https://html5up.net/
[MIT License]: https://opensource.org/licenses/MIT
[GNU Lesser General Public License v3]: https://www.gnu.org/licenses/lgpl-3.0.txt
[OpenSSL]: https://www.openssl.org
[OpenSSL and SSLeay licenses]:https://www.openssl.org/source/license.html
[Doxygen]: http://www.stack.nl/~dimitri/doxygen/
[Abbrevio]: https://github.com/CanScribe/Abbrevio
[Visual Studio 2022]: https://www.visualstudio.com/vs/community/
[Qt Visual Studio Tools]: https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022
[XMiLib]: https://github.com/xmichelo/XMiLib
[NSIS]: http://nsis.sourceforge.net/Main_Page
