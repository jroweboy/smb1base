object FormBankCHR: TFormBankCHR
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'CHR Bank Manager'
  ClientHeight = 409
  ClientWidth = 414
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Image2: TImage
    Left = 2
    Top = 150
    Width = 256
    Height = 256
    OnMouseDown = Image2MouseDown
    OnMouseEnter = Image2MouseEnter
    OnMouseLeave = Image2MouseLeave
    OnMouseMove = Image2MouseMove
  end
  object ListBox1: TListBox
    Left = 264
    Top = 2
    Width = 148
    Height = 404
    ExtendedSelect = False
    ItemHeight = 13
    TabOrder = 0
    OnClick = ListBox1Click
    OnMouseMove = ListBox1MouseMove
  end
  object GroupBox1: TGroupBox
    Left = 2
    Top = 2
    Width = 173
    Height = 145
    Caption = 'Current work set'
    TabOrder = 1
    object Image1: TImage
      Left = 3
      Top = 14
      Width = 128
      Height = 128
      OnMouseDown = Image1MouseDown
      OnMouseEnter = Image1MouseEnter
      OnMouseLeave = Image1MouseLeave
      OnMouseMove = Image1MouseMove
    end
    object btnA: TSpeedButton
      Left = 136
      Top = 24
      Width = 17
      Height = 20
      GroupIndex = 1
      Caption = 'A'
      OnClick = btnAClick
    end
    object btnB: TSpeedButton
      Left = 153
      Top = 24
      Width = 17
      Height = 20
      GroupIndex = 1
      Caption = 'B'
      OnClick = btnAClick
    end
    object btn4k: TSpeedButton
      Left = 136
      Top = 62
      Width = 34
      Height = 20
      GroupIndex = 2
      Down = True
      Caption = '4k'
      OnClick = btn4kClick
    end
    object btn1k: TSpeedButton
      Left = 136
      Top = 102
      Width = 34
      Height = 20
      GroupIndex = 2
      Caption = '1k'
      OnClick = btn2kClick
    end
    object btn2k: TSpeedButton
      Left = 136
      Top = 82
      Width = 34
      Height = 20
      GroupIndex = 2
      Caption = '2k'
      OnClick = btn2kClick
    end
    object Label1: TLabel
      Left = 138
      Top = 47
      Width = 26
      Height = 12
      Caption = 'Select'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 138
      Top = 10
      Width = 14
      Height = 12
      Caption = 'Set'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btn512b: TSpeedButton
      Left = 136
      Top = 122
      Width = 17
      Height = 20
      GroupIndex = 2
      Caption = #189
      OnClick = btn2kClick
    end
    object btn256b: TSpeedButton
      Left = 153
      Top = 122
      Width = 17
      Height = 20
      GroupIndex = 2
      Caption = #188
      OnClick = btn2kClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 178
    Top = 2
    Width = 80
    Height = 145
    Caption = 'Bank list item'
    TabOrder = 2
    object Insert1: TSpeedButton
      Left = 4
      Top = 15
      Width = 72
      Height = 19
      Caption = 'Insert'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Insert1Click
    end
    object Remove1: TSpeedButton
      Left = 4
      Top = 36
      Width = 42
      Height = 19
      Caption = 'Remove'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Remove1Click
    end
    object Duplicate1: TSpeedButton
      Tag = 1
      Left = 4
      Top = 57
      Width = 72
      Height = 19
      Caption = 'Duplicate'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Insert1Click
    end
    object Up1: TSpeedButton
      Left = 4
      Top = 78
      Width = 35
      Height = 19
      Caption = 'Up'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Up1Click
    end
    object Down1: TSpeedButton
      Left = 41
      Top = 78
      Width = 35
      Height = 19
      Caption = 'Down'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Down1Click
    end
    object Clear1: TSpeedButton
      Left = 48
      Top = 36
      Width = 28
      Height = 19
      Caption = 'Clear'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Clear1Click
    end
    object Copy1: TSpeedButton
      Left = 4
      Top = 120
      Width = 35
      Height = 19
      Caption = 'Copy'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Paste1: TSpeedButton
      Left = 41
      Top = 120
      Width = 35
      Height = 19
      Caption = 'Paste'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object SpeedButton1: TSpeedButton
      Left = 3
      Top = 99
      Width = 72
      Height = 19
      Caption = 'Rename'
      OnClick = SpeedButton1Click
    end
  end
  object DrawTimer: TTimer
    Enabled = False
    Interval = 50
    OnTimer = DrawTimerTimer
    Left = 196
    Top = 201
  end
  object OpenByFileAssociationMakeListTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = OpenByFileAssociationMakeListTimerTimer
    Left = 171
    Top = 259
  end
end
