object FormSwapPatternColour: TFormSwapPatternColour
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Swap pattern co&lours'
  ClientHeight = 135
  ClientWidth = 300
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 300
    Height = 135
    Align = alClient
    TabOrder = 0
    ExplicitWidth = 298
    ExplicitHeight = 195
    object GroupBox3: TGroupBox
      Left = 3
      Top = 8
      Width = 138
      Height = 53
      Caption = 'Order'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 13
    end
    object GroupBox2: TGroupBox
      Left = 3
      Top = 65
      Width = 161
      Height = 64
      Caption = 'Apply to'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 12
    end
    object RadioButton4K: TRadioButton
      Left = 10
      Top = 106
      Width = 80
      Height = 17
      Caption = 'current set'
      TabOrder = 5
      OnClick = RadioButton4KClick
    end
    object RadioButton8K: TRadioButton
      Left = 88
      Top = 106
      Width = 69
      Height = 17
      Caption = 'sets A+B'
      TabOrder = 6
      OnClick = RadioButton4KClick
    end
    object RadioButtonSelection: TRadioButton
      Left = 10
      Top = 84
      Width = 61
      Height = 17
      Caption = 'selection'
      Checked = True
      TabOrder = 7
      TabStop = True
      OnClick = RadioButton4KClick
    end
    object ButtonSwap: TButton
      Left = 222
      Top = 103
      Width = 70
      Height = 25
      Caption = 'OK'
      TabOrder = 8
      OnClick = ButtonSwapClick
    end
    object ButtonCancel: TButton
      Left = 222
      Top = 74
      Width = 70
      Height = 25
      Caption = 'Cancel'
      TabOrder = 9
      OnClick = ButtonCancelClick
    end
    object Button1: TButton
      Left = 10
      Top = 24
      Width = 30
      Height = 25
      TabOrder = 0
      OnMouseDown = Button1MouseDown
    end
    object Button2: TButton
      Tag = 1
      Left = 41
      Top = 24
      Width = 30
      Height = 25
      TabOrder = 1
    end
    object Button3: TButton
      Tag = 2
      Left = 72
      Top = 24
      Width = 30
      Height = 25
      TabOrder = 2
    end
    object Button4: TButton
      Tag = 3
      Left = 103
      Top = 24
      Width = 30
      Height = 25
      TabOrder = 3
    end
    object ButtonReset: TButton
      Left = 222
      Top = 14
      Width = 70
      Height = 16
      Caption = 'Reset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      TabStop = False
      OnClick = ButtonResetClick
    end
    object ButtonDarker: TButton
      Left = 222
      Top = 34
      Width = 32
      Height = 16
      Hint = 
        'This swap setting will shift color indexes to the left, making g' +
        'raphics darker'
      Caption = 'Dark'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 10
      TabStop = False
      OnClick = ButtonDarkerClick
    end
    object ButtonBrighter: TButton
      Left = 255
      Top = 34
      Width = 37
      Height = 16
      Hint = 
        'This swap setting will shift color indexes to the right, making ' +
        'graphics brighter'
      Caption = 'Bright'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 11
      TabStop = False
      OnClick = ButtonBrighterClick
    end
    object Button5: TButton
      Left = 148
      Top = 38
      Width = 41
      Height = 20
      Caption = 'Flip 3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 14
      OnClick = Button5Click
    end
    object Button6: TButton
      Left = 190
      Top = 38
      Width = 25
      Height = 20
      Caption = '4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 15
      OnClick = Button6Click
    end
    object CheckBox1: TCheckBox
      Left = 223
      Top = 53
      Width = 73
      Height = 17
      Caption = 'Preview'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 16
      OnClick = CheckBox1Click
    end
    object Button7: TButton
      Left = 182
      Top = 14
      Width = 33
      Height = 20
      Caption = '>'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 17
      OnClick = Button7Click
    end
    object Button8: TButton
      Left = 148
      Top = 14
      Width = 33
      Height = 20
      Caption = '<'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 18
      OnClick = Button8Click
    end
    object Button9: TButton
      Left = 170
      Top = 96
      Width = 46
      Height = 16
      Caption = 'cap +'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 19
      OnClick = Button9Click
    end
    object Button10: TButton
      Left = 170
      Top = 112
      Width = 46
      Height = 16
      Caption = 'cap -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 20
      OnClick = Button10Click
    end
    object Button11: TButton
      Left = 170
      Top = 64
      Width = 46
      Height = 16
      Caption = 'wrap +'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 21
      OnClick = Button11Click
    end
    object Button12: TButton
      Left = 170
      Top = 80
      Width = 46
      Height = 16
      Caption = 'wrap -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 22
      OnClick = Button12Click
    end
  end
end
