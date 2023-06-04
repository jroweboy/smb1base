object FormSwapColors: TFormSwapColors
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Swap subpal colour order'
  ClientHeight = 195
  ClientWidth = 298
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 298
    Height = 195
    Align = alClient
    TabOrder = 0
    object GroupBox4: TGroupBox
      Left = 3
      Top = 127
      Width = 213
      Height = 62
      Caption = 'Apply to subpalettes'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 16
      object RadioPalCurrent: TRadioButton
        Tag = 1
        Left = 13
        Top = 21
        Width = 92
        Height = 17
        Caption = 'current set'
        TabOrder = 0
        OnClick = CheckBoxPalClick
        OnMouseDown = RadioPalCurrentMouseDown
        OnMouseUp = RadioPalCurrentMouseUp
      end
      object RadioPalAll: TRadioButton
        Tag = 1
        Left = 118
        Top = 20
        Width = 79
        Height = 17
        Caption = 'sets A...D'
        TabOrder = 1
        OnClick = CheckBoxPalClick
        OnMouseDown = RadioPalCurrentMouseDown
        OnMouseUp = RadioPalCurrentMouseUp
      end
      object RadioPalOne: TRadioButton
        Tag = 1
        Left = 13
        Top = 42
        Width = 67
        Height = 15
        Caption = 'subpal:'
        TabOrder = 2
        OnClick = CheckBoxPalClick
        OnMouseDown = RadioPalCurrentMouseDown
        OnMouseUp = RadioPalCurrentMouseUp
      end
      object RadioPalNone: TRadioButton
        Tag = 1
        Left = 118
        Top = 42
        Width = 65
        Height = 15
        Caption = 'none'
        TabOrder = 3
        OnClick = CheckBoxPalClick
        OnMouseDown = RadioPalCurrentMouseDown
        OnMouseUp = RadioPalCurrentMouseUp
      end
      object ButtonWhichSubpal: TButton
        Left = 76
        Top = 39
        Width = 30
        Height = 20
        Caption = '0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnMouseDown = Button1MouseDown
        OnMouseEnter = ButtonWhichSubpalMouseEnter
        OnMouseLeave = ButtonResetMouseLeave
      end
    end
    object GroupBox3: TGroupBox
      Left = 3
      Top = 13
      Width = 138
      Height = 53
      Caption = 'Order'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 14
    end
    object GroupBox2: TGroupBox
      Left = 3
      Top = 65
      Width = 213
      Height = 62
      Caption = 'Apply to patterns'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 13
    end
    object RadioButton4K: TRadioButton
      Left = 16
      Top = 84
      Width = 113
      Height = 17
      Caption = 'current set'
      Checked = True
      TabOrder = 5
      TabStop = True
      OnClick = RadioButton4KClick
    end
    object RadioButton8K: TRadioButton
      Left = 121
      Top = 84
      Width = 88
      Height = 17
      Caption = 'sets A...B'
      TabOrder = 6
      OnClick = RadioButton4KClick
    end
    object RadioButtonSelection: TRadioButton
      Left = 16
      Top = 104
      Width = 83
      Height = 17
      Caption = 'selection'
      TabOrder = 7
      OnClick = RadioButton4KClick
    end
    object ButtonSwap: TButton
      Left = 222
      Top = 163
      Width = 70
      Height = 25
      Caption = 'OK'
      TabOrder = 8
      OnClick = ButtonSwapClick
      OnMouseEnter = ButtonSwapMouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object ButtonCancel: TButton
      Left = 222
      Top = 134
      Width = 70
      Height = 25
      Caption = 'Cancel'
      TabOrder = 9
      OnClick = ButtonCancelClick
      OnMouseEnter = ButtonCancelMouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object Button1: TButton
      Left = 10
      Top = 32
      Width = 30
      Height = 25
      TabOrder = 0
      OnMouseDown = Button1MouseDown
      OnMouseEnter = Button3MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object Button2: TButton
      Tag = 1
      Left = 41
      Top = 32
      Width = 30
      Height = 25
      TabOrder = 1
      OnMouseDown = Button1MouseDown
      OnMouseEnter = Button3MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object Button3: TButton
      Tag = 2
      Left = 72
      Top = 32
      Width = 30
      Height = 25
      TabOrder = 2
      OnMouseDown = Button1MouseDown
      OnMouseEnter = Button3MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object Button4: TButton
      Tag = 3
      Left = 103
      Top = 32
      Width = 30
      Height = 25
      TabOrder = 3
      OnMouseDown = Button1MouseDown
      OnMouseEnter = Button3MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object ButtonReset: TButton
      Left = 222
      Top = 20
      Width = 70
      Height = 22
      Caption = 'Reset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = ButtonResetClick
      OnMouseEnter = ButtonResetMouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object ButtonDarker: TButton
      Left = 222
      Top = 43
      Width = 32
      Height = 22
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
      OnClick = ButtonDarkerClick
      OnMouseEnter = ButtonDarkerMouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object ButtonBrighter: TButton
      Left = 255
      Top = 43
      Width = 37
      Height = 22
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
      OnClick = ButtonBrighterClick
      OnMouseEnter = ButtonBrighterMouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object RadioPatternNone: TRadioButton
      Left = 121
      Top = 104
      Width = 69
      Height = 17
      Caption = 'none'
      TabOrder = 12
      OnClick = RadioButton4KClick
    end
    object Button5: TButton
      Left = 148
      Top = 43
      Width = 41
      Height = 22
      Caption = 'Flip 3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 15
      OnClick = Button5Click
      OnMouseEnter = Button5MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object Button6: TButton
      Left = 190
      Top = 43
      Width = 25
      Height = 22
      Caption = '4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 17
      OnClick = Button6Click
      OnMouseEnter = Button6MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object CheckBox1: TCheckBox
      Left = 222
      Top = 111
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
      TabOrder = 18
      OnClick = CheckBox1Click
    end
    object GroupBox5: TGroupBox
      Left = 222
      Top = 65
      Width = 72
      Height = 44
      Caption = 'Col 0 from'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 19
      object ButtonCol0: TButton
        Left = 3
        Top = 16
        Width = 65
        Height = 22
        Caption = 'subpal 0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnMouseDown = Button1MouseDown
        OnMouseEnter = ButtonCol0MouseEnter
        OnMouseLeave = ButtonResetMouseLeave
      end
    end
    object Button7: TButton
      Left = 182
      Top = 20
      Width = 33
      Height = 22
      Caption = '>'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 20
      OnClick = Button7Click
      OnMouseEnter = Button7MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
    object Button8: TButton
      Left = 148
      Top = 20
      Width = 33
      Height = 22
      Caption = '<'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 21
      OnClick = Button8Click
      OnMouseEnter = Button8MouseEnter
      OnMouseLeave = ButtonResetMouseLeave
    end
  end
end
