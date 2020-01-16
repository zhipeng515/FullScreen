# FullScreen


* 使用说明
```C++

  // 继承FullScreenSupport
  class WTLWindow :
		public ATL::CWindowImpl<WTLWindow>,
		public FullScreenSupport<WTLWindow>
	{
	public:
		BEGIN_MSG_MAP(WTLWindow)
			CHAIN_MSG_MAP(FullScreenSupport<WTLWindow>)
		END_MSG_MAP()
	};
  
  // 指定需要全屏的窗口
  wtlWindow_.SetFullScreenClient(pClient);
  // 指定提示窗口
  wtlWindow_.SetFullScreenTip(hFullScreenTipWnd);
  
  // 全屏切换
  wtlWindow_.SwitchFullScreen();
```

* 依赖
[CommonFunction](https://github.com/zhipeng515/CommonFunction)
