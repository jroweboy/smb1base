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
  OnCreate = FormCreate
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
    OnClick = RadioButtonNormalClick
    ExplicitWidth = 264
    ExplicitHeight = 140
    object BtnWdtInc: TSpeedButton
      Left = 172
      Top = 22
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
      Left = 230
      Top = 22
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
      Left = 172
      Top = 60
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
      Left = 230
      Top = 60
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
    object ButtonCancel: TButton
      Left = 203
      Top = 117
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = ButtonCancelClick
    end
    object ButtonOK: TButton
      Left = 120
      Top = 117
      Width = 75
      Height = 25
      Caption = 'OK'
      TabOrder = 1
      OnClick = ButtonOKClick
    end
    object EditWidth: TEdit
      Left = 172
      Top = 37
      Width = 33
      Height = 21
      TabOrder = 2
      Text = '32'
      OnClick = EditWidthClick
      OnExit = EditWidthExit
      OnKeyPress = EditWidthKeyPress
    end
    object EditHeight: TEdit
      Left = 230
      Top = 37
      Width = 33
      Height = 21
      TabOrder = 3
      Text = '30'
      OnClick = EditWidthClick
      OnExit = EditHeightExit
      OnKeyPress = EditWidthKeyPress
    end
    object UpDownWidth: TUpDown
      Left = 205
      Top = 37
      Width = 17
      Height = 21
      Associate = EditWidth
      Min = 4
      Max = 4096
      Increment = 4
      Position = 32
      TabOrder = 4
      Thousands = False
    end
    object UpDownHeight: TUpDown
      Left = 263
      Top = 37
      Width = 17
      Height = 21
      Associate = EditHeight
      Min = 4
      Max = 4096
      Increment = 4
      Position = 30
      TabOrder = 5
      Thousands = False
    end
    object RadioButtonNormal: TRadioButton
      Left = 16
      Top = 22
      Width = 146
      Height = 17
      Caption = 'Standard 32x30 (1 screen)'
      Checked = True
      TabOrder = 6
      TabStop = True
      OnClick = RadioButtonNormalClick
    end
    object RadioButtonUser: TRadioButton
      Left = 16
      Top = 40
      Width = 113
      Height = 17
      Caption = 'User defined (map)'
      TabOrder = 7
      OnClick = RadioButtonNormalClick
    end
    object CheckBoxClear: TCheckBox
      Left = 16
      Top = 96
      Width = 129
      Height = 17
      Caption = 'Clear existing contents'
      TabOrder = 8
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
