object FormSetSize: TFormSetSize
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Set canvas size'
  ClientHeight = 148
  ClientWidth = 293
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 293
    Height = 148
    Align = alClient
    TabOrder = 0
    object BtnWdtInc: TSpeedButton
      Left = 16
      Top = 20
      Width = 33
      Height = 14
      Caption = 'w+32'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnWdtIncClick
    end
    object BtnHgtInc: TSpeedButton
      Left = 86
      Top = 20
      Width = 33
      Height = 14
      Caption = 'h+30'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnHgtIncClick
    end
    object BtnWdtDec: TSpeedButton
      Left = 16
      Top = 58
      Width = 33
      Height = 14
      Caption = 'w -32'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnWdtDecClick
    end
    object BtnHgtDec: TSpeedButton
      Left = 86
      Top = 58
      Width = 33
      Height = 14
      Caption = 'h -30'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnHgtDecClick
    end
    object Btn32x30: TSpeedButton
      Left = 160
      Top = 38
      Width = 119
      Height = 16
      Caption = '1 full NES screen (32x30)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Btn32x30Click
    end
    object BtnThisSession: TSpeedButton
      Left = 160
      Top = 20
      Width = 119
      Height = 16
      Caption = 'Reset to current dimensions'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnThisSessionClick
    end
    object SpeedButton1: TSpeedButton
      Left = 160
      Top = 56
      Width = 119
      Height = 16
      Caption = '4 full NES screens (64x60)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object ButtonCancel: TButton
      Left = 120
      Top = 117
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 3
      TabStop = False
      OnClick = ButtonCancelClick
    end
    object ButtonOK: TButton
      Left = 205
      Top = 117
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      TabOrder = 0
      TabStop = False
      OnClick = ButtonOKClick
    end
    object EditWidth: TEdit
      Left = 16
      Top = 35
      Width = 33
      Height = 21
      TabOrder = 1
      Text = '32'
      OnClick = EditWidthClick
      OnExit = EditWidthExit
      OnKeyPress = EditWidthKeyPress
    end
    object EditHeight: TEdit
      Left = 86
      Top = 35
      Width = 33
      Height = 21
      TabOrder = 2
      Text = '30'
      OnClick = EditWidthClick
      OnExit = EditHeightExit
      OnKeyPress = EditWidthKeyPress
    end
    object UpDownWidth: TUpDown
      Left = 49
      Top = 35
      Width = 17
      Height = 21
      Associate = EditWidth
      Min = 4
      Max = 4096
      Increment = 4
      Position = 32
      TabOrder = 5
      Thousands = False
    end
    object UpDownHeight: TUpDown
      Left = 119
      Top = 35
      Width = 17
      Height = 21
      Associate = EditHeight
      Min = 4
      Max = 4096
      Increment = 4
      Position = 30
      TabOrder = 6
      Thousands = False
    end
    object CheckBoxClear: TCheckBox
      Left = 16
      Top = 96
      Width = 129
      Height = 17
      Caption = 'Clear existing contents'
      TabOrder = 4
    end
  end
  object CheckNullTile: TCheckBox
    Left = 16
    Top = 78
    Width = 236
    Height = 17
    Caption = 'Use null tile for canvas extension or clearing'
    Checked = True
    State = cbChecked
    TabOrder = 1
  end
end
