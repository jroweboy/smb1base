object FormMetaspritePlaybackRules: TFormMetaspritePlaybackRules
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Metasprite Playback rules'
  ClientHeight = 223
  ClientWidth = 488
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 3
    Top = 2
    Width = 386
    Height = 56
    Caption = 'Bitcode tags'
    TabOrder = 0
    object RadioButton1: TRadioButton
      Left = 8
      Top = 33
      Width = 150
      Height = 17
      Caption = 'Use start tags; 1 call tag'
      TabOrder = 0
      OnClick = RadioButton1Click
      OnMouseEnter = RadioButton1MouseEnter
    end
    object RadioButton2: TRadioButton
      Left = 8
      Top = 16
      Width = 150
      Height = 17
      Caption = 'Imply start; use 2 call tags'
      Checked = True
      TabOrder = 1
      OnClick = RadioButton2Click
      OnMouseEnter = RadioButton2MouseEnter
    end
    object chkReplaceCall1: TCheckBox
      Left = 170
      Top = 16
      Width = 118
      Height = 17
      TabStop = False
      Caption = 'Replace <call> with'
      Checked = True
      State = cbChecked
      TabOrder = 2
      OnClick = chkReplaceCall1Click
      OnMouseEnter = chkReplaceCall1MouseEnter
    end
    object chkReplaceCall2: TCheckBox
      Left = 170
      Top = 33
      Width = 120
      Height = 17
      TabStop = False
      Caption = 'Replace <call2> with <revert>'
      Checked = True
      State = cbChecked
      TabOrder = 3
      OnClick = chkReplaceCall2Click
      OnMouseEnter = chkReplaceCall1MouseEnter
    end
    object ComboBoxCall1: TComboBox
      Left = 294
      Top = 12
      Width = 86
      Height = 21
      AutoDropDown = True
      AutoCloseUp = True
      ItemHeight = 13
      ItemIndex = 2
      TabOrder = 4
      TabStop = False
      Text = '<half secs>'
      OnChange = ComboBoxCall1Change
      Items.Strings = (
        '<revert>'
        '<seconds>'
        '<half secs>')
    end
    object ComboBoxCall2: TComboBox
      Left = 294
      Top = 33
      Width = 86
      Height = 21
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 5
      TabStop = False
      Text = '<revert>'
      OnChange = ComboBoxCall2Change
      Items.Strings = (
        '<revert>'
        '<seconds>'
        '<half secs>')
    end
  end
  object GroupBox2: TGroupBox
    Left = 3
    Top = 58
    Width = 484
    Height = 164
    Caption = 'Explanation'
    TabOrder = 1
    object StaticText1: TStaticText
      Left = 11
      Top = 17
      Width = 470
      Height = 144
      AutoSize = False
      Caption = 'StaticText1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object GroupBox3: TGroupBox
    Left = 387
    Top = 2
    Width = 100
    Height = 56
    Caption = 'Cap duration'
    TabOrder = 2
    object RadioDontCap: TRadioButton
      Left = 5
      Top = 16
      Width = 48
      Height = 17
      Caption = 'Don'#39't'
      Checked = True
      TabOrder = 0
      OnClick = RadioDontCapClick
      OnMouseEnter = RadioDontCapClick
    end
    object Radio63Cap: TRadioButton
      Left = 56
      Top = 16
      Width = 40
      Height = 17
      Caption = '63'
      TabOrder = 1
      OnClick = RadioDontCapClick
      OnMouseEnter = RadioDontCapClick
    end
    object Radio127Cap: TRadioButton
      Left = 5
      Top = 33
      Width = 40
      Height = 17
      Caption = '127'
      TabOrder = 2
      OnClick = RadioDontCapClick
      OnMouseEnter = RadioDontCapClick
    end
    object Radio255Cap: TRadioButton
      Left = 50
      Top = 33
      Width = 40
      Height = 17
      Caption = '255'
      TabOrder = 3
      OnClick = RadioDontCapClick
      OnMouseEnter = RadioDontCapClick
    end
  end
end
