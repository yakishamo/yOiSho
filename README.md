# yOiSho
自作OSをやります．
コマンドは無くなりました．全部ユーザランドで実装してね．

## 起動方法
qemu, clang, lld, x86\_64-w64-mingw32-gccがあれば動きます多分．  
ovmf/以下にOVMF\_CODE.fdとOVMF\_VARS.fdを配置してください．
makeでコンパイル，  
make runで実行できます．  

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
* kmalloc.h
  * kmalloc()とkfree()の実装，あとカーネルのヒープ領域の確保
* interrupt.h
  * 割り込み処理に必要な構造体定義等
* serial.h
	* シリアル通信の簡単な実装
	* ただ単に送受信を実装しているのみ
	* ターミナルとして使っていくのはserial.hの範囲外
