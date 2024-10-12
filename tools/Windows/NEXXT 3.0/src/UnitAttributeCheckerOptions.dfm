object FormAttrChecker: TFormAttrChecker
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Attribute Overlay method'
  ClientHeight = 106
  ClientWidth = 203
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnMouseDown = FormMouseDown
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 2
    Top = 0
    Width = 199
    Height = 34
    Caption = 'Style'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object SpeedButton1: TSpeedButton
      Left = 5
      Top = 12
      Width = 36
      Height = 17
      GroupIndex = 1
      Caption = 'Classic'
      OnClick = SpeedButton1Click
    end
    object SpeedButton2: TSpeedButton
      Tag = 2
      Left = 79
      Top = 12
      Width = 41
      Height = 17
      GroupIndex = 1
      Down = True
      Caption = 'Solid 1'
      OnClick = SpeedButton1Click
    end
    object SpeedButton3: TSpeedButton
      Tag = 1
      Left = 42
      Top = 12
      Width = 36
      Height = 17
      GroupIndex = 1
      Caption = 'Stripes'
      OnClick = SpeedButton1Click
    end
    object SpeedButton4: TSpeedButton
      Tag = 4
      Left = 158
      Top = 12
      Width = 36
      Height = 17
      GroupIndex = 1
      Caption = 'Pal ID'
      OnClick = SpeedButton1Click
    end
    object SpeedButton5: TSpeedButton
      Tag = 3
      Left = 121
      Top = 12
      Width = 36
      Height = 17
      GroupIndex = 1
      Caption = 'Solid 2'
      OnClick = SpeedButton1Click
    end
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 34
    Width = 199
    Height = 34
    Caption = 'Opacity'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Label1: TLabel
      Left = 6
      Top = 14
      Width = 15
      Height = 11
      Caption = '5%'
    end
    object Label2: TLabel
      Left = 170
      Top = 14
      Width = 25
      Height = 11
      Caption = '100%'
    end
    object TrackBar1: TTrackBar
      Left = 20
      Top = 12
      Width = 150
      Height = 20
      Max = 100
      Min = 5
      PageSize = 5
      Frequency = 5
      Position = 55
      ShowSelRange = False
      TabOrder = 0
      TabStop = False
      ThumbLength = 14
      OnChange = TrackBar1Change
    end
  end
  object GroupBox3: TGroupBox
    Left = 2
    Top = 68
    Width = 154
    Height = 34
    Caption = 'Separation'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object Label3: TLabel
      Left = 6
      Top = 14
      Width = 15
      Height = 11
      Caption = '0%'
    end
    object Label4: TLabel
      Left = 120
      Top = 14
      Width = 25
      Height = 11
      Caption = '100%'
    end
    object TrackBar2: TTrackBar
      Left = 20
      Top = 12
      Width = 100
      Height = 20
      LineSize = 5
      Max = 30
      PageSize = 5
      Frequency = 5
      Position = 15
      ShowSelRange = False
      TabOrder = 0
      TabStop = False
      ThumbLength = 14
      OnChange = TrackBar2Change
    end
  end
  object GroupBox4: TGroupBox
    Left = 150
    Top = 68
    Width = 51
    Height = 34
    Caption = 'Rotation'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object Label5: TLabel
      Left = 6
      Top = 14
      Width = 5
      Height = 11
      Caption = '0'
    end
    object SpeedButton6: TSpeedButton
      Left = 18
      Top = 11
      Width = 23
      Height = 10
      Caption = '+'
      OnClick = SpeedButton6Click
    end
    object SpeedButton7: TSpeedButton
      Left = 18
      Top = 21
      Width = 23
      Height = 10
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton7Click
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 88
    Top = 56
    object Resetpresets1: TMenuItem
      Caption = 'Reset presets'
      OnClick = Resetpresets1Click
    end
  end
end
