# インストール計測の詳細

* [1. インストール計測の実装](#track_install_basic)
* [2. インストール計測の実装(オプション指定)](#track_install_optional)
* [3. その他のインストール計測実装例](#track_install_other)
* [4. Webタグを用いた計測](#track_webtag)

<div id="track_install_basic"></div>
## 1. インストール計測の実装

onLaunchメソッドを利用することで、インストール計測を行うことができます。Cookie計測を利用する場合には、外部ブラウザが起動されます。この際、外部ブラウザの遷移先をonLaunchの引数にURL文字列を指定することができます。

プロジェクトのソースコードを編集し、アプリケーションの起動時に呼び出されるActivityに対して、次の通り実装を行ってください。


```java
import co.cyberz.fox.FoxTrack;

@Override
public void onCreate(Bundle savedInstanceState) {
	FoxTrack.onLaunch();
}
```

> ※ 引数を指定せずonLaunchメソッドを呼び出すと、F.O.X管理画面上での設定内容が優先されます。Cookie計測の際のリダイレクト先URLを指定する場合は、以降の説明をご確認ください。

> ※ onLaunchメソッドは、特に理由がない限りはアプリケーションの起動時に呼び出されるActivityのonCreateメソッド内に実装してください。それ以外の箇所に実装された場合にはインストール数が正確に計測できない場合があります。<br>
アプリケーションの起動時に呼び出されるActivityのonCreate、メソッド内に実装していない状態でインストール成果型の広告を実施する際には、必ず広告代理店もしくは媒体社の担当にその旨を伝えてください。正確に計測が行えない状態でインストール成果型の広告を実施された際には、計測されたインストール数以上の広告費の支払いを求められる恐れがあります。

![sendConversion01](./img01.png)

<div id="track_install_optional"></div>
## 2. インストール計測の実装(オプション指定)

インストール計測が完了したことをコールバックで受け取りたい場合、特定のURLヘ遷移させる場合や、アプリケーションで動的にURLを生成したい場合には、以下の[FoxTrackOptionクラス](./sdk_api/README.md#foxtrackoption)を使用します。<br>

```java
@Override
public void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);

  // 初回起動のインストール計測
  FoxTrackOption option = new FoxTrackOption();
  option.setRedirectUrl("myapp://top")
        .setBuid(getUserId())
        .setOptOut(true)
        .setTrackingStateListener(new FoxTrackOption.TrackingStateListerner() {
           @Override
           public void onComplete() {
             showTutorialDialog();
           }
        });
  FoxEvent.onLaunch(option);
}
```

> 上記のサンプルコードでは、リダイレクト先・BUID・オプトアウトの有無・計測完了のコールバックを受け取る処理の実装例となっています。<br>TrackingStateListernerをセットした上で計測処理が完了するとonCompleteメソッドが呼ばれますので、インストール計測完了直後に実行したい処理はこちらに実装してください。

> オプトアウトを有効にした場合、その後そのユーザーを広告の配信対象から外すことが可能です。<br>
尚、オプトアウトはユーザーに対しオプトアウトの意思表示を選択させるような機能をアプリ内で実装している場合に有効です。

> F.O.X SDKのAPI仕様は[こちら](./sdk_api/README.md)でご確認ください。

<div id="track_install_other"></div>
## 3. その他のインストール計測実装例

`FoxTrack.onLaunch()`をApplication継承クラスに実装することで、起動計測系の処理を集約することが可能となります。<br>
但し、Activityが呼ばれる前に動作するためFoxTrackOptionを用いたBUIDの指定やオプトアウトの指定は難しい場合があります。そのため、Fingerprint計測やInstallReferrer計測などのCookie計測を行わない場合に有効です。

```java
import android.app.Application;
import co.cyberz.common.FoxConfig;
import co.cyberz.fox.FoxTrack;

public class YourApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();

        // アクティベーション処理
        private int FOX_APP_ID = 発行されたアプリID;
        private String FOX_APP_KEY = "発行されたAPP_KEY";
        private String FOX_APP_SALT = "発行されたAPP_SALT";
        new FoxConfig(this, FOX_APP_ID, FOX_APP_KEY, FOX_APP_SALT).activate();

        // アプリケーションのライフサイクルの検知
        registerActivityLifecycleCallbacks(new ApplicationLifeCycleCallbacks());
    }


    private static final class ApplicationLifeCycleCallbacks implements ActivityLifecycleCallbacks {

	    @Override
	    public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
	      // インストール計測
	      FoxTrack.onLaunch();
	    }

	    @Override
	    public void onActivityStarted(Activity activity) {
	    }

	    @Override
	    public void onActivityResumed(Activity activity) {
	      // セッショントラッキング
	      FoxTrack.session();
	      // リエンゲージメント計測
	      FoxTrack.onDeeplinkLaunch(activity);
	    }

	    @Override
	    public void onActivityPaused(Activity activity) {
	    }

	    @Override
	    public void onActivityStopped(Activity activity) {
	    }

	    @Override
	    public void onActivitySaveInstanceState(Activity activity, Bundle outState) {
	    }

	    @Override
	    public void onActivityDestroyed(Activity activity) {
	    }
  }
}
```

<div id="track_webtag"></div>
## 4. Webタグを用いたLTV計測

会員登録や商品購入等がWebページで行われる場合に、imgタグを利用してLTV計測を利用することができます。<br>
<br>
F.O.XのLTV計測は、外部ブラウザ、アプリ内WebViewの両方に対応しています。外部ブラウザの場合にはtrackEventByBrowserメソッド、アプリ内WebViewの場合にはtrackEventByWebViewメソッドを利用することで、F.O.XがLTV計測に必要な情報をブラウザのCookieに記録します。

### 4.1 外部ブラウザでのLTV計測

アプリケーションから外部ブラウザを起動し、外部ブラウザで表示したWebページでタグ計測を行う場合には、trackEventByBrowserメソッドを利用して外部ブラウザを起動してください。<br>
引数には外部ブラウザでアクセスするURLを文字列で指定します。

```java
import co.cyberz.fox.FoxTrack;

...

FoxTrack.trackEventByBrowser("http://www.mysite.com/event/");
```

### 4.2 アプリ内WebViewでのLTV計測

ユーザーの遷移がWebView内で行われる場合には、trackEventByWebViewメソッドを用いることで計測することができます。WebViewが生成される箇所で下記コードを実行してください。WebViewが複数回生成・破棄される場合には、生成される度にtrackEventByWebViewメソッドが実行されるようにしてください。内部的にandroid.webkit.CookieManagerとandroid.webkit.CookieSyncManagerを利用してCookieをセットします。<br>
また、Android LよりWebViewインスタンスごとにサードパーティCookieの書き込みの許可を行うため引数にWebViewを必須としています。

```java
import co.cyberz.fox.FoxTrack;

@Override
public void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);

  WebView mWebView = (WebView) findViewById(R.id.sample_webview);
	FoxTrack.trackEventByWebView(mWebView);
	mWebView.loadUrl("http://www.mysite.com/event/");
}
```

### 4.3 タグの実装

LTVの成果地点となるページに計測タグを実装してください。計測タグは弊社管理者より連絡致します。<br>
タグに利用するパラメータは以下の通りです。

|パラメータ名|必須|備考|
|:-----|:-----|:-----|
|_buyer|必須|広告主を識別するID。<br />管理者より連絡しますので、その値を入力してください。|
|_cvpoint|必須|成果地点を識別するID。<br />管理者より連絡しますので、その値を入力してください。|
|_price|オプション|課金額。課金計測時に設定してください。|
|_currency|オプション|半角英字3文字の通貨コード。<br />課金計測時に設定してください。<br />通貨が設定されていない場合、_priceをJPY(日本円)として扱います。|
|_buid|オプション|半角英数字64文字まで。<br />会員IDなどユーザー毎にユニークな値を保持する場合にご使用ください。|

> _currencyには[ISO 4217](http://ja.wikipedia.org/wiki/ISO_4217)で定義された通貨コードを指定してください。

---
[トップ](/4.x/lang/ja/README.md)
