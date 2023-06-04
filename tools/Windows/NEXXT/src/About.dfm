object AboutBox: TAboutBox
  Left = 200
  Top = 108
  BorderStyle = bsDialog
  Caption = 'About'
  ClientHeight = 213
  ClientWidth = 343
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 225
    Height = 197
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 0
    object ProductName: TLabel
      Left = 8
      Top = 8
      Width = 62
      Height = 28
      Caption = 'NEXXT'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Bauhaus 93'
      Font.Style = []
      ParentFont = False
      IsControl = True
    end
    object StaticText1: TStaticText
      Left = 8
      Top = 42
      Width = 58
      Height = 17
      Caption = 'StaticText1'
      TabOrder = 0
    end
  end
  object OKButton: TButton
    Left = 239
    Top = 180
    Width = 96
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = OKButtonClick
  end
  object btnItch: TButton
    Left = 239
    Top = 8
    Width = 96
    Height = 25
    Caption = 'NEXXT@ Itch.io'
    TabOrder = 2
    OnClick = btnItchClick
  end
  object btnCommunity: TButton
    Left = 239
    Top = 36
    Width = 96
    Height = 25
    Caption = 'NEXXT forums'
    TabOrder = 3
    OnClick = btnCommunityClick
  end
  object btnTwitter: TButton
    Left = 239
    Top = 120
    Width = 96
    Height = 25
    Caption = 'Twitter'
    TabOrder = 4
    OnClick = btnTwitterClick
  end
  object btnPatreon: TButton
    Left = 239
    Top = 148
    Width = 96
    Height = 25
    Caption = 'Patreon'
    TabOrder = 5
    OnClick = btnPatreonClick
  end
  object btnShiru: TButton
    Left = 239
    Top = 64
    Width = 96
    Height = 25
    Caption = 'Shiru'#39's page'
    TabOrder = 6
    OnClick = btnShiruClick
  end
  object Button1: TButton
    Left = 239
    Top = 92
    Width = 96
    Height = 25
    Caption = 'Mastodon'
    TabOrder = 7
    OnClick = Button1Click
  end
end
