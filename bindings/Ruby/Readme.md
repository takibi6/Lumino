﻿Lumino for Ruby
====================

extconf.rb をつかうとどうしても in-source ビルドになってしまう。(ビルドツリーとソースツリーを分けられない)

いろいろ管理しづらいので、ビルド時は一度 build フォルダをつくり、その中に .cpp とかをコピーしてからビルドしている。

ビルドするときは x86 の Ruby と DevKit を使うこと。
