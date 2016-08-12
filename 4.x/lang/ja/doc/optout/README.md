## （オプション）オプトアウトの実装
広告会社によってターゲティング広告に利用されないことをユーザーに選択させることが可能です。アプリケーションの起動時において、プライバシーポリシーや利用規約を表示するダイアログでユーザーがオプトアウトを選択した場合、効果測定の結果の通知と共に、F.O.Xが広告会社に対してそのユーザーがオプトアウトを選択したことを通知します。
オプトアウトに対応する場合は、以下の通り「[インストールの計測の実装](/4.x/lang/ja/doc/track_install/README.md#track_install_optional)」で実装する際の[`FoxTrackOption`](./4.x/lang/ja/doc/sdk_api/README.md#foxtrackoption)で`OptOutをtrue`に設定を行ってください。

```java
// 初回起動のインストール計測
FoxTrackOption option = new FoxTrackOption();
option.setRedirectUrl("myapp://top")
			.setBuid(getUserId())
			.setOptOut(true);
FoxEvent.onLaunch(option);
```

---
[トップ](/4.x/lang/ja/README.md)
