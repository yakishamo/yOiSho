# yOiSho
自作OSをやります.

## 起動方法
qemu, clang, lld, x86\_64-w64-mingw32-gccがあれば動きます多分.  
makeでコンパイル,   
make runで実行できます. 

## コマンド一覧
* ls
  * ファイル一覧表示
* rm [ファイル名]
  * ファイル削除
* echo [文字列...]
  * 入力された引数を列挙して表示
* cat [ファイル名]
  * ファイルの内容のダンプ
* edit [ファイル名]
  * ファイルの編集．ファイル名が存在しない場合は新規作成する．ファイル名指定なしではエラー．

## 構成
* main.c
  * カーネルのエントリポイントmainの定義を含むファイル
* frame.h
  * ターミナルへの文字出力含む画面描画関係
* hankaku.c
  * Asciiフォントファイル
* memory.h
  * ページ単位のメモリ管理
* segment.h
  * 工事中
* string.h
  * 文字列処理(strlen()など)
* keyboard.h
  * キーコードからAscii文字への変換など
* terminal.h
  * ターミナルの処理を行う関数．コマンド定義なども含む．
* cursor.h
  * 文字入力時に使うカーソルの定義
* parse.h
  * 文字列のトークナイズ処理．主にターミナルに入力されたコマンドの解釈に使う
* file.h
  * 簡易的なファイルシステム．固定長のサイズのファイルを固定長分だけ作れる
* editor.h
  * ファイル編集コマンドeditの本体
* asmfunc.asm
  * アセンブリでしか書けない処理をまとめたファイル．基本的にインラインアセンブラは使わない．**アセンブリ書く方がカッコイイので．**
* window.h
  * window作らずターミナルだけで作っていこうと思っていたのに気づいたらwindowクラスのようなものを作っており敗北
  * GUIは後半で実装するので封印
